#ifndef SPTAGLIB_VECTORINDEXWRAPPER_H
#define SPTAGLIB_VECTORINDEXWRAPPER_H

#include <utility>

#include "inc/Core/VectorIndex.h"
#include "inc/Core/SPANN/Index.h"


using namespace SPTAG;
using namespace SPTAG::SPANN;

namespace IVF {
    class VectorIndexWrapper {
    public:
        VectorIndexWrapper(const std::shared_ptr<SPTAG::VectorIndex>& vecIndex);

        void getHeadIDs(std::vector<int> &headIDs, void *query_vector);

        void getPostingList(SizeType headID, std::istringstream *postingItemStream);

        ~VectorIndexWrapper() = default;

        int getVecLen();

    private:
        std::shared_ptr<SPTAG::VectorIndex> vecIndex;
    };



}


#endif //SPTAGLIB_VECTORINDEXWRAPPER_H
