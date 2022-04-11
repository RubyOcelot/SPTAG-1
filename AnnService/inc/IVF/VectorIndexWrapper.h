#ifndef SPTAGLIB_VECTORINDEXWRAPPER_H
#define SPTAGLIB_VECTORINDEXWRAPPER_H

#include <utility>

#include "inc/Core/VectorIndex.h"
#include "inc/Core/SPANN/Index.h"
#include "inc/IVF/interfaces/KeywordIndexWrapper.h"

using namespace SPTAG;
using namespace SPTAG::SPANN;

namespace IVF {
    class VectorIndexWrapper:public KeywordIndexWrapper {
    public:
        VectorIndexWrapper(const std::shared_ptr<SPTAG::VectorIndex>& vecIndex);
        VectorIndexWrapper(const VectorIndexWrapper& vecIndexWrapper);

        void getHeadIDs(std::vector<int> &headIDs, void *query_vector);

        void getPostingList(SizeType headID, std::istringstream *postingItemStream);

        ~VectorIndexWrapper() = default;

        int getVecLen();

        int addVector(const void *p_data, SizeType p_vectorNum);

        void deleteVector(void *target_vector);

        bool updateAllFinished() override;

    private:
        std::shared_ptr<SPTAG::VectorIndex> vecIndex;
    };



}


#endif //SPTAGLIB_VECTORINDEXWRAPPER_H
