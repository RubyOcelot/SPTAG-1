#ifndef IVF_COLLECTOR_H
#define IVF_COLLECTOR_H

#include "ForwardDefine.h"
#include "utils/DocPriorityQueue.h"
#include "TopDocs.h"

namespace IVF {
    class Collector {
    private:
        //collects the results of a search. maintaining a priority queue for top docs.
        DocPriorityQueue &pq;

    public:
        Collector(int n);

        void collect(DocId docId, float score);

        //return docs in pq
        TopDocs topDocs();

        void debug_print(){
            pq.debug_print();
        }
    };
}

#endif //IVF_COLLECTOR_H
