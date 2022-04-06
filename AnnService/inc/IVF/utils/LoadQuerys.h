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
    }

    class QuerysWrap {
    public:
        int numQueries;
        std::shared_ptr<VectorSet> querySet;

        int loadData(SPANN::Options *p_opts) {
            LOG(Helper::LogLevel::LL_Info, "Start loading QuerySet...\n");
            std::shared_ptr<Helper::ReaderOptions> queryOptions(
                    new Helper::ReaderOptions(p_opts->m_valueType, p_opts->m_dim, p_opts->m_queryType,
                                              p_opts->m_queryDelimiter));
            auto queryReader = Helper::VectorSetReader::CreateInstance(queryOptions);


            if (ErrorCode::Success != queryReader->LoadFile(p_opts->m_queryPath)) {
                LOG(Helper::LogLevel::LL_Error, "Failed to read query file.\n");
                exit(1);
            }
            querySet = queryReader->GetVectorSet();
            numQueries = querySet->Count();
        }

        int print_vector_by_id(SizeType p_vectorID){
            void* vec=querySet->GetVector(p_vectorID);
            if(vec==nullptr){
                //TODO error
                return -1;
            }
            print_vector(vec,querySet->Dimension(),querySet->GetValueType());
        }
    };

}
#endif //SPTAGLIB_LOADQUERYS_H
