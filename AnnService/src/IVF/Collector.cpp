#include "inc/IVF/Collector.h"


namespace IVF {

    bool DocWithScoreLessThanFunc(const std::shared_ptr<DocWithScore> &a,const std::shared_ptr<DocWithScore> &b){
        return a->score<b->score;
    }

    Collector::Collector(int n) : pq(std::make_unique<SizedPriorityQueue<DocWithScore>>(n, &DocWithScoreLessThanFunc)) {
    }

    void Collector::collect(DocId docId, float score) {
        pq->insertWithOverflow(std::make_shared<DocWithScore>(docId, score));
    }

    TopDocs Collector::topDocs() {
        TopDocs td = TopDocs();
        while (!pq->isEmpty()) {
            td.add(*(pq->pop()));
        }
        td.reverse();
        return td;
    }

}
