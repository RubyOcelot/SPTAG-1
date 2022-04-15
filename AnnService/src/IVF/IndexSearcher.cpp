#include "inc/IVF/IndexSearcher.h"
#include "inc/IVF/interfaces/Query.h"
#include "inc/IVF/utils/TopDocs.h"
#include "inc/IVF//utils/Bits.h"
#include "inc/IVF//BulkScorer.h"
#include "inc/IVF/DefaultVectorIndexConfig.h"

namespace IVF {

    TopDocs IndexSearcher::search(Query &query, int n) {
        //create pq with n, pass it to collector
        std::unique_ptr<Scorer> scorer = query.getScorer();
        BulkScorer bulkScorer = BulkScorer(std::move(scorer));
        auto *collector = new Collector(n);
        Bits liveDocs = getLiveDocs();
        bulkScorer.scoreAll(collector, liveDocs);
        TopDocs td = collector->topDocs();
        delete collector;
        return td;
    }

    Bits IndexSearcher::getLiveDocs() {
        //TODO TESTC
        return Bits(10);
    }

    void IndexSearcher::getCollectionStatistics(ScoreScheme *scoreScheme) {
        //TODO impl
        scoreScheme->collectionStatisticsLoader("");
    }

    IndexSearcher::IndexSearcher(const std::string &path, const std::shared_ptr<IndexConfig>& config) {

        config->init(path, *this);
    }

    int IndexSearcher::addKeyword(std::unique_ptr<Keyword> kw) {
        //TODO error
        kw->addToIndex();
        return 0;
    }

    bool IndexSearcher::updateAllFinished() {
        for(const auto& iter:indexCollection){
            if(!iter->updateAllFinished())
                return false;
        }
        return true;
    }
}
