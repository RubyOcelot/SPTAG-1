#ifndef SPTAGLIB_TERMINDEX_H
#define SPTAGLIB_TERMINDEX_H

#include "inc/IVF/interfaces/KeywordIndex.h"
#include "inc/IVF/interfaces/KeyValueIO.h"
#include "inc/IVF/Term.h"
#include "inc/IVF/interfaces/StringHeadIndex.h"
#include "inc/IVF/utils/TermSetDataHolder.h"

namespace IVF {

    class TermIndex : public KeywordIndex {
    public:
        bool updateAllFinished() override;
        void close() override;
        HeadIDType getHeadID(const Term &term, std::string *rt_stat);
        void getPostingList(HeadIDType headID, std::istringstream *postingItemStream);
        void setPostingList(HeadIDType headID,  const std::string &value);
        void buildIndex(std::unique_ptr<TermSetDataHolder> dataHolder,int threadNum);
    private:
        std::unique_ptr<KeyValueIO> db;
        std::unique_ptr<StringHeadIndex> head_index;
        std::unique_ptr<CollectionStatistic> collectionStatistic;
    };


}

#endif //SPTAGLIB_TERMINDEX_H
