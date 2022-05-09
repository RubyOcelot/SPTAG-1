#ifndef IVF_DOCPRIORITYQUEUE_H
#define IVF_DOCPRIORITYQUEUE_H

#include "DocWithScore.h"


namespace IVF {
    template<class T>
    class DocPriorityQueue {
    public:
        DocPriorityQueue(int maxSize,bool (*lessThanFunc)(const std::shared_ptr<T> &a,const std::shared_ptr<T> &ab));

//        float getMinScore();

        bool isEmpty();

        std::shared_ptr<T> pop();

        std::shared_ptr<T> top();

        std::shared_ptr<T> insertWithOverflow(const std::shared_ptr<T> &element);

        // linear time cost
        bool remove(const std::shared_ptr<T> &element);

        void clear();

        int getSize();

        int getMaxSize();

        void debug_print();

    private:
        int maxSize;
        std::unique_ptr<std::shared_ptr<T>[]> heap;
        int size = 0;

        bool upHeap(int origPos);

        bool downHeap(int origPos);

        std::shared_ptr<T> updateTop();

        bool lessThan(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b);

        bool (*lessThanFunc)(const std::shared_ptr<T> &a,const std::shared_ptr<T> &b);

// add element to bottom of heap when heap is no full
        std::shared_ptr<T> add(const std::shared_ptr<T> &element);
    };
}

#endif //IVF_DOCPRIORITYQUEUE_H
