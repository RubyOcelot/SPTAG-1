#ifndef SPTAGLIB_VECTORINDEXWRAPPER_H
#define SPTAGLIB_VECTORINDEXWRAPPER_H

#include <utility>

#include "inc/Core/VectorIndex.h"
#include "inc/Core/SPANN/Index.h"


using namespace SPTAG;
using namespace SPTAG::SPANN;

namespace IVF{
    template<class T>
    class VectorIndexWrapper{
    public:
        VectorIndexWrapper( const std::shared_ptr<SPTAG::VectorIndex>& vecIndex);
        void getHeadIDs(std::vector<int> & headIDs,std::shared_ptr<std::vector<T>> query_vector){

            auto SPANNIndex=(SPTAG::SPANN::Index<T>*)(vecIndex.get());
            SPANN::Options& p_opts = *(SPANNIndex->GetOptions());


            QueryResult p_query=QueryResult(nullptr, max(p_opts.m_resultNum, p_opts.m_searchInternalResultNum), false);

            T* target_vec=new T[query_vector->size()];
            for(int i=0;i<query_vector->size();i++){
                target_vec[i]=query_vector->at(i);
            }
            p_query.SetTarget(target_vec);
            p_query.Reset();

            auto memory_index= (SPANNIndex->GetMemoryIndex().get());
            memory_index->SearchIndex(p_query);

            float limitDist = p_query.GetResult(0)->Dist * p_opts.m_maxDistRatio;
            for(int i=0;i<p_query.GetResultNum();i++){
                auto res = p_query.GetResult(i);
                if (res->VID == -1 || (limitDist > 0.1 && res->Dist > limitDist)) break;
                headIDs.push_back(p_query.GetResult(i)->VID);
            }
            //m_vectorTranslateMap?

        }
        void getPostingList(SizeType headID, std::istringstream *postingItemStream){
            std::string posting;
            auto SPANNIndex=(SPTAG::SPANN::Index<T>*)(vecIndex.get());
            SPANNIndex->GetDiskIndex()->SearchIndex(headID, posting);
            postingItemStream->str(posting);
        }
        ~VectorIndexWrapper()=default;
        int getVecLen(){
            auto SPANNIndex=(SPTAG::SPANN::Index<T>*)(vecIndex.get());
            return SPANNIndex->GetOptions()->m_dim;
        }
    private:
        std::shared_ptr<SPTAG::VectorIndex>  vecIndex;
    };



    template<class T>
    VectorIndexWrapper<T>::VectorIndexWrapper(const std::shared_ptr<SPTAG::VectorIndex>& vecIndex) {
        this->vecIndex=std::move(vecIndex);
    }

    template class VectorIndexWrapper<float>;
    template class VectorIndexWrapper<int8_t>;

}


#endif //SPTAGLIB_VECTORINDEXWRAPPER_H
