#include "inc/IVF/Collector.h"


namespace IVF {
    Collector::Collector(int n) : pq(*(new DocPriorityQueue(n))) {
    }

    void Collector::collect(DocId docId, float score) {
        pq.insertWithOverflow(DocWithScore(docId, score));
    }

    TopDocs Collector::topDocs() {
        TopDocs td = TopDocs();
        while (!pq.isEmpty()) {
            td.add(*(pq.pop()));
        }
        return td;
    }

}
