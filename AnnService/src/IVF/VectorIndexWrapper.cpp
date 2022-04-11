#include "inc/IVF/VectorIndexWrapper.h"

namespace IVF{

    VectorIndexWrapper::VectorIndexWrapper(const std::shared_ptr<SPTAG::VectorIndex>& vecIndex) {
        this->vecIndex = vecIndex;
    }

    void VectorIndexWrapper::getHeadIDs(std::vector<int> &headIDs, void *query_vector) {


        SPANN::Options* p_opts;
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                    p_opts = ((SPANN::Index<Type>*)vecIndex.get())->GetOptions();\
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType

        QueryResult p_query = QueryResult(nullptr, max(p_opts->m_resultNum, p_opts->m_searchInternalResultNum),
                                          false);

        p_query.SetTarget(query_vector);
        p_query.Reset();


        std::shared_ptr<VectorIndex> memory_index;
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                     memory_index=((SPANN::Index<Type>*)vecIndex.get())->GetMemoryIndex(); \
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType

        memory_index->SearchIndex(p_query);

        float limitDist = p_query.GetResult(0)->Dist * p_opts->m_maxDistRatio;
        for (int i = 0; i < p_query.GetResultNum(); i++) {
            auto res = p_query.GetResult(i);
            if (res->VID == -1 || (limitDist > 0.1 && res->Dist > limitDist)) break;
            headIDs.push_back(p_query.GetResult(i)->VID);
        }
        //m_vectorTranslateMap?

    }



    void VectorIndexWrapper::getPostingList(SizeType headID, std::istringstream *postingItemStream) {
        std::string posting;
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                     ((SPANN::Index<Type>*)vecIndex.get())->GetDiskIndex()->SearchIndex(headID, posting); \
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        postingItemStream->str(posting);
    }


    int VectorIndexWrapper::getVecLen() {
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                     return ((SPANN::Index<Type>*)vecIndex.get())->GetOptions()->m_dim; \
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        return -1;
    }

    int VectorIndexWrapper::addVector(const void *p_data, SizeType p_vectorNum) {
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                     ((SPANN::Index<Type>*)vecIndex.get())->AddIndex(p_data, p_vectorNum, vecIndex->GetFeatureDim(), nullptr); \
                     return 0;  \
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        return 0;
    }

    bool VectorIndexWrapper::updateAllFinished() {
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                     return ((SPANN::Index<Type>*)vecIndex.get())->AllFinished(); \
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        return false;
    }

    VectorIndexWrapper::VectorIndexWrapper(const VectorIndexWrapper &vecIndexWrapper) {
        vecIndex=vecIndexWrapper.vecIndex;
    }

}