#include "inc/IVF/IndexSearcher.h"
#include "inc/IVF/interfaces/Query.h"
#include "inc/IVF/TopDocs.h"
#include "inc/IVF//utils/Bits.h"
#include "inc/IVF//BulkScorer.h"
#include "inc/IVF/DefaultVectorIndexConfig.h"
#include "inc/IVF/RocksDbKeyValueIO.h"
#include "inc/IVF/KeyVector.h"

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

    IndexSearcher::IndexSearcher(std::string dir) {

        //TODO read indexconfig from file
        //TODO template type
        auto indexConfig = DefaultVectorIndexConfig<int8_t>();
        indexConfig.init(dir);

        //TODO init SPTAG


    }
}
