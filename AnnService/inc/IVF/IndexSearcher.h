#ifndef IVF_INDEXSEARCHER_H
#define IVF_INDEXSEARCHER_H

#include <string>
#include <memory>
#include "interfaces/ScoreScheme.h"
#include "interfaces/Keyword.h"
#include "interfaces/KeywordIndex.h"
#include "inc/IVF/utils/TopDocs.h"
#include "inc/IVF/interfaces/Query.h"
#include "inc/IVF/utils/Bits.h"
#include "inc/IVF/interfaces/IndexConfig.h"

namespace IVF {
    class IndexSearcher {
        //top n
        //use BulkScorer with query
    public:
        explicit IndexSearcher(const std::string &path, const std::shared_ptr<IndexConfig>& config);

        TopDocs search(Query &query, int n);

        int addKeyword(std::unique_ptr<Keyword> kw);

        Bits getLiveDocs();

        void getCollectionStatistics(ScoreScheme *scoreScheme);

        bool updateAllFinished();

        std::vector<std::shared_ptr<KeywordIndex>> indexCollection;
    private:
    };
}

#endif //IVF_INDEXSEARCHER_H
