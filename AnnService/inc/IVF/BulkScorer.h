#ifndef IVF_BULKSCORER_H
#define IVF_BULKSCORER_H

#include "ForwardDefine.h"
#include "Collector.h"
#include "interfaces/Scorer.h"

namespace IVF {
    class BulkScorer {
    public:
        BulkScorer(std::unique_ptr<Scorer> scorer);

        std::unique_ptr<Scorer> scorer;

        void scoreAll(Collector *, const Bits &);

        int score(Collector *collector, Bits liveDocs, DocId min, DocId max);
        //iter through docs and get score with scorer, call collect()
    };
}



#endif //IVF_BULKSCORER_H
