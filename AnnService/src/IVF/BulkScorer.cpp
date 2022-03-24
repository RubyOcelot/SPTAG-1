#include "inc/IVF/BulkScorer.h"
#include "inc/IVF/utils/Bits.h"

namespace IVF {
    void BulkScorer::scoreAll(Collector *collector, const Bits &liveDocs) {
        //collector.setScorer(scorer);
        DocId docId = scorer->getCurrentId();
        while (docId != -1) {
            if (liveDocs.get(docId)) {
                collector->collect(docId, scorer->score());
            }
            docId = scorer->next();
        }
    }

//TODO
    int BulkScorer::score(Collector *collector, Bits liveDocs, DocId min, DocId max) {
        return -1;
    }

    BulkScorer::BulkScorer(std::unique_ptr<Scorer> scorer) : scorer(std::move(scorer)) {

    }

}