// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <iostream>

#include "inc/Core/Common.h"
#include "inc/Core/VectorIndex.h"
#include "inc/Core/SPANN/Index.h"
#include "inc/Helper/SimpleIniReader.h"
#include "inc/Helper/VectorSetReader.h"
#include "inc/Helper/StringConvert.h"
#include "inc/Core/Common/TruthSet.h"

#include "inc/SSDServing/main.h"
#include "inc/SSDServing/Utils.h"
#include "inc/SSDServing/SSDIndex.h"

using namespace SPTAG;
//using namespace SPTAG::SSDServing;

namespace IVF {

    std::shared_ptr<VectorIndex> SetupSPTAGIndex(std::map<std::string, std::map<std::string, std::string>> *config_map,
                                                 const char *configurationPath);

    const std::string SEC_BASE = "Base";
    const std::string SEC_SELECT_HEAD = "SelectHead";
    const std::string SEC_BUILD_HEAD = "BuildHead";
    const std::string SEC_BUILD_SSD_INDEX = "BuildSSDIndex";
    const std::string SEC_SEARCH_SSD_INDEX = "SearchSSDIndex";

    std::shared_ptr<VectorIndex> SetupSPTAGIndex(std::map<std::string, std::map<std::string, std::string>> *config_map,
                                                 const char *configurationPath) {

        VectorValueType valueType;
        DistCalcMethod distCalcMethod;

        Helper::IniReader iniReader;
        iniReader.LoadIniFile(configurationPath);
        (*config_map)["Base"] = iniReader.GetParameters("Base");
        (*config_map)["SelectHead"] = iniReader.GetParameters("SelectHead");
        (*config_map)["BuildHead"] = iniReader.GetParameters("BuildHead");
        (*config_map)["BuildSSDIndex"] = iniReader.GetParameters("BuildSSDIndex");

        valueType = iniReader.GetParameter("Base", "ValueType", valueType);
        distCalcMethod = iniReader.GetParameter("Base", "DistCalcMethod", distCalcMethod);
        bool buildSSD = iniReader.GetParameter("BuildSSDIndex", "isExecute", false);

        for (auto& KV : iniReader.GetParameters("SearchSSDIndex")) {
            std::string param = KV.first, value = KV.second;
            if (buildSSD && Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "BuildSsdIndex")) continue;
            if (buildSSD && Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "isExecute")) continue;
            if (Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "PostingPageLimit")) param = "SearchPostingPageLimit";
            if (Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "InternalResultNum")) param = "SearchInternalResultNum";
            (*config_map)["BuildSSDIndex"][param] = value;
        }

        std::shared_ptr<VectorIndex> index = VectorIndex::CreateInstance(IndexAlgoType::SPANN, valueType);
        if (index == nullptr) {
            LOG(Helper::LogLevel::LL_Error, "Cannot create Index with ValueType %s!\n", (*config_map)["Base"]["ValueType"].c_str());
            exit(1);
        }

        for (auto& sectionKV : *config_map) {
            for (auto& KV : sectionKV.second) {
                index->SetParameter(KV.first, KV.second, sectionKV.first);
            }
        }

        if (index->BuildIndex() != ErrorCode::Success) {
            LOG(Helper::LogLevel::LL_Error, "Failed to build index.\n");
            exit(1);
        }

        SPANN::Options* opts = nullptr;

#define DefineVectorValueType(Name, Type) \
            if (index->GetVectorValueType() == VectorValueType::Name) { \
            opts = ((SPANN::Index<Type>*)index.get())->GetOptions(); \
            } \

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType

        if (opts == nullptr) {
            LOG(Helper::LogLevel::LL_Error, "Cannot get options.\n");
            exit(1);
        }

        if (opts->m_generateTruth)
        {
            LOG(Helper::LogLevel::LL_Info, "Start generating truth. It's maybe a long time.\n");
            if (COMMON::DistanceUtils::Quantizer) valueType = VectorValueType::UInt8;
            std::shared_ptr<Helper::ReaderOptions> vectorOptions(new Helper::ReaderOptions(valueType, opts->m_dim, opts->m_vectorType, opts->m_vectorDelimiter));
            auto vectorReader = Helper::VectorSetReader::CreateInstance(vectorOptions);
            if (ErrorCode::Success != vectorReader->LoadFile(opts->m_vectorPath))
            {
                LOG(Helper::LogLevel::LL_Error, "Failed to read vector file.\n");
                exit(1);
            }
            std::shared_ptr<Helper::ReaderOptions> queryOptions(new Helper::ReaderOptions(valueType, opts->m_dim, opts->m_queryType, opts->m_queryDelimiter));
            auto queryReader = Helper::VectorSetReader::CreateInstance(queryOptions);
            if (ErrorCode::Success != queryReader->LoadFile(opts->m_queryPath))
            {
                LOG(Helper::LogLevel::LL_Error, "Failed to read query file.\n");
                exit(1);
            }
            auto vectorSet = vectorReader->GetVectorSet();
            auto querySet = queryReader->GetVectorSet();
            if (distCalcMethod == DistCalcMethod::Cosine) vectorSet->Normalize(opts->m_iSSDNumberOfThreads);

            omp_set_num_threads(opts->m_iSSDNumberOfThreads);

#define DefineVectorValueType(Name, Type) \
            if (opts->m_valueType == VectorValueType::Name) { \
            COMMON::TruthSet::GenerateTruth<Type>(querySet, vectorSet, opts->m_truthPath, \
                distCalcMethod, opts->m_resultNum, opts->m_truthType); \
            } \

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType

            LOG(Helper::LogLevel::LL_Info, "End generating truth.\n");
        }

        return index;
    }
}
