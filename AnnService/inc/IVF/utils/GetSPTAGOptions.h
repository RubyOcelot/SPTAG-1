#ifndef SPTAGLIB_GETSPTAGOPTIONS_H
#define SPTAGLIB_GETSPTAGOPTIONS_H

#include <string>
#include <map>
#include "inc/Core/Common.h"
#include "inc/Core/VectorIndex.h"
#include "inc/Core/SPANN/Options.h"

using namespace SPTAG;

namespace IVF{

    SPANN::Options* getSPTAGOptions(const char* configurationPath) {
        auto config_map=new std::map<std::string, std::map<std::string, std::string>>;
        VectorValueType valueType;
        DistCalcMethod distCalcMethod;
    
        bool searchSSD = false;
        Helper::IniReader iniReader;
        iniReader.LoadIniFile(configurationPath);
        (*config_map)["Base"] = iniReader.GetParameters("Base");
        (*config_map)["SelectHead"] = iniReader.GetParameters("SelectHead");
        (*config_map)["BuildHead"] = iniReader.GetParameters("BuildHead");
        (*config_map)["BuildSSDIndex"] = iniReader.GetParameters("BuildSSDIndex");
    
        valueType = iniReader.GetParameter("Base", "ValueType", valueType);
        bool buildSSD = iniReader.GetParameter("BuildSSDIndex", "isExecute", false);
    
        for (auto &KV: iniReader.GetParameters("SearchSSDIndex")) {
            std::string param = KV.first, value = KV.second;
            if (buildSSD && Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "BuildSsdIndex")) continue;
            if (buildSSD && Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "isExecute")) continue;
            if (Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "PostingPageLimit")) param = "SearchPostingPageLimit";
            if (Helper::StrUtils::StrEqualIgnoreCase(param.c_str(), "InternalResultNum")) param = "SearchInternalResultNum";
            (*config_map)["BuildSSDIndex"][param] = value;
        }
    
        std::shared_ptr <VectorIndex> index = VectorIndex::CreateInstance(IndexAlgoType::SPANN, valueType);
        if (index == nullptr) {
            LOG(Helper::LogLevel::LL_Error, "Cannot create Index with ValueType %s!\n",
                (*config_map)["Base"]["ValueType"].c_str());
            return nullptr;
        }
    
        for (auto &sectionKV: *config_map) {
            for (auto &KV: sectionKV.second) {
                index->SetParameter(KV.first, KV.second, sectionKV.first);
            }
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
            return nullptr;
        }
        delete config_map;
        return new SPANN::Options(*opts);
    }
}

#endif //SPTAGLIB_GETSPTAGOPTIONS_H
