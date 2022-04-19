#ifndef SPTAGLIB_TERMINDEX_H
#define SPTAGLIB_TERMINDEX_H

#include "inc/IVF/interfaces/KeywordIndex.h"
#include "inc/IVF/interfaces/KeyValueIO.h"
#include "inc/IVF/Term.h"

namespace IVF {

    class TermIndex : public KeywordIndex {
    public:
        bool updateAllFinished() override;
        void close() override;
        HeadIDType getHeadID(const Term& term);
        void getPostingList(HeadIDType headID, std::istringstream *postingItemStream);
    private:
        std::unique_ptr<KeyValueIO> db;

    };


}

#endif //SPTAGLIB_TERMINDEX_H
