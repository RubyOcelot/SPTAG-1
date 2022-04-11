#ifndef SPTAGLIB_LOADQUERYS_H
#define SPTAGLIB_LOADQUERYS_H

#include <vector>
#include <set>
#include "inc/Core/Common.h"
#include "inc/Core/Common/CommonUtils.h"
#include "inc/Core/Common/TruthSet.h"
#include "inc/Core/SPANN/Options.h"
#include "inc/Helper/VectorSetReader.h"

using namespace SPTAG;

namespace IVF {

    int print_vector(void *vec, DimensionType dim, VectorValueType typeName) {
        for (int i = 0; i < dim; i++) {

#define DefineVectorValueType(Name, Type) \
    if (typeName == VectorValueType::Name) { \
        std::cout<<+(*(((Type*)vec)+i))<<" "; \
    } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        }
        std::cout<<std::endl;
        return 0;
    }

    class VectorReaderWrap {
    public:
        int numVectors;
        std::shared_ptr<VectorSet> p_vectorSet;

        int loadQueryData(SPANN::Options *p_opts) {
            LOG(Helper::LogLevel::LL_Info, "Start loading QuerySet...\n");
            std::shared_ptr<Helper::ReaderOptions> queryOptions(
                    new Helper::ReaderOptions(p_opts->m_valueType, p_opts->m_dim, p_opts->m_queryType,
                                              p_opts->m_queryDelimiter));
            auto queryReader = Helper::VectorSetReader::CreateInstance(queryOptions);


            if (ErrorCode::Success != queryReader->LoadFile(p_opts->m_queryPath)) {
                LOG(Helper::LogLevel::LL_Error, "Failed to read query file.\n");
                exit(1);
            }
            p_vectorSet = queryReader->GetVectorSet();
            numVectors = p_vectorSet->Count();
            return 0;
        }

        int loadVectorData(SPANN::Options *p_opts){
            LOG(Helper::LogLevel::LL_Info, "Start loading VectorSet...\n");
            std::shared_ptr<Helper::ReaderOptions> vectorOptions(new Helper::ReaderOptions(p_opts->m_valueType, p_opts->m_dim, p_opts->m_vectorType, p_opts->m_vectorDelimiter, false));
            auto vectorReader = Helper::VectorSetReader::CreateInstance(vectorOptions);
            if (p_opts->m_vectorPath.empty())
            {
                LOG(Helper::LogLevel::LL_Info, "Vector file is empty. Skipping loading.\n");
            }
            else {
                if (ErrorCode::Success != vectorReader->LoadFile(p_opts->m_vectorPath))
                {
                    LOG(Helper::LogLevel::LL_Error, "Failed to read vector file.\n");
//                    return ErrorCode::Fail;
                    return -1;
                }
                p_vectorSet=vectorReader->GetVectorSet();
                numVectors = p_vectorSet->Count();
            }
            return 0;
        }

        int loadFullVectorData(SPANN::Options *p_opts){
            LOG(Helper::LogLevel::LL_Info, "Start loading full VectorSet...\n");
            std::shared_ptr<Helper::ReaderOptions> vectorOptions(new Helper::ReaderOptions(p_opts->m_valueType, p_opts->m_dim, p_opts->m_vectorType, p_opts->m_vectorDelimiter, false));
            auto vectorReader = Helper::VectorSetReader::CreateInstance(vectorOptions);
            if (p_opts->m_fullVectorPath.empty())
            {
                LOG(Helper::LogLevel::LL_Info, "Full vector file is empty. Skipping loading.\n");
            }
            else {
                if (ErrorCode::Success != vectorReader->LoadFile(p_opts->m_fullVectorPath))
                {
                    LOG(Helper::LogLevel::LL_Error, "Failed to read full vector file.\n");
//                    return ErrorCode::Fail;
                    return -1;
                }
                p_vectorSet=vectorReader->GetVectorSet();
                numVectors = p_vectorSet->Count();
                if (p_opts->m_distCalcMethod == DistCalcMethod::Cosine) p_vectorSet->Normalize(p_opts->m_searchThreadNum);
                LOG(Helper::LogLevel::LL_Info, "\nLoad VectorSet(%d,%d).\n", p_vectorSet->Count(), p_vectorSet->Dimension());
            }
            return 0;
        }

        int print_vector_by_id(SizeType p_vectorID){
            void* vec=p_vectorSet->GetVector(p_vectorID);
            if(vec==nullptr){
                //TODO error
                return -1;
            }
            print_vector(vec, p_vectorSet->Dimension(), p_vectorSet->GetValueType());
            return 0;
        }
    };

}
#endif //SPTAGLIB_LOADQUERYS_H
