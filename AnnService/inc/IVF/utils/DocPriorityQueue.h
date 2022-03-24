#ifndef IVF_DOCPRIORITYQUEUE_H
#define IVF_DOCPRIORITYQUEUE_H

#include "DocWithScore.h"


namespace IVF {
    class DocPriorityQueue {
    public:
        DocPriorityQueue(int maxSize);

        float getMinScore();

        bool isEmpty();

        DocWithScore *pop();

        DocWithScore *top();

        DocWithScore *insertWithOverflow(const DocWithScore &element);

        // linear time cost
        bool remove(const DocWithScore &element);

        void clear();

        int getSize();

        int getMaxSize();

        ~DocPriorityQueue();

    private:
        int maxSize;
        DocWithScore *heap;
        int size = 0;

        bool upHeap(int origPos);

        bool downHeap(int origPos);

        DocWithScore updateTop();

        bool lessThan(const DocWithScore &a, const DocWithScore &b);


// add element to bottom of heap when heap is no full
        DocWithScore add(const DocWithScore &element);
    };
}

#endif //IVF_DOCPRIORITYQUEUE_H
