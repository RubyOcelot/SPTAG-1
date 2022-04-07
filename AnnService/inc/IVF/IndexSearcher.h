#ifndef IVF_INDEXSEARCHER_H
#define IVF_INDEXSEARCHER_H

#include <string>
#include "interfaces/ScoreScheme.h"
#include "interfaces/Keyword.h"

namespace IVF {
    class IndexSearcher {
        //top n
        //use BulkScorer with query
    public:
        TopDocs search(Query &query, int n);

        int addKeyword(Keyword &kw);

        Bits getLiveDocs();

        void getCollectionStatistics(ScoreScheme *scoreScheme);

        explicit IndexSearcher(const std::string &path);
    };
}

#endif //IVF_INDEXSEARCHER_H
