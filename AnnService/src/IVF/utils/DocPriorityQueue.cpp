#include "inc/IVF/utils/DocPriorityQueue.h"


namespace IVF {
    DocPriorityQueue::DocPriorityQueue(int maxSize) : maxSize(maxSize) {
        int heapSize;

        if (maxSize == 0) {
            // allocate 1 extra to avoid if statement in top()
            heapSize = 2;
        } else {
            if ((maxSize < 0)) {
                //TODO exception
                //TODO maxSize upper bound
            }
            //start from 1
            heapSize = maxSize + 1;
        }
        heap = new DocWithScore[heapSize];
    }

    float DocPriorityQueue::getMinScore() {
        return top()->score;
    }

    DocWithScore *DocPriorityQueue::top() {
        //TBD isEmpty?
        return new DocWithScore(heap[1]);
    }

    DocWithScore DocPriorityQueue::add(const DocWithScore &element) {
        int index = size + 1;
        heap[index] = element;
        size = index;
        upHeap(index);
        return heap[1];
    }

    bool DocPriorityQueue::lessThan(const DocWithScore &a, const DocWithScore &b) {
        return a.score < b.score;
    }

// return dropped element if heap is full
    DocWithScore *DocPriorityQueue::insertWithOverflow(const DocWithScore &element) {
        if (size < maxSize) {
            add(element);
            return nullptr;
        } else {
            if (size > 0 && lessThan(heap[1], element)) {
                auto *ret = new DocWithScore(heap[1]);
                heap[1] = element;
                updateTop();
                return ret;
            } else {
                return new DocWithScore(element);
            }
        }
    }

    DocWithScore DocPriorityQueue::updateTop() {
        downHeap(1);
        return heap[1];
    }

    bool DocPriorityQueue::upHeap(int origPos) {
        int cur = origPos;
        auto origNode = heap[origPos];
        int up = cur >> 1;
        while (up > 0 && lessThan(origNode, heap[up])) {
            heap[cur] = heap[up];
            cur = up;
            up = up >> 1;
        }
        heap[cur] = origNode;
        return cur != origPos;
    }

    bool DocPriorityQueue::downHeap(int origPos) {
        int cur = origPos;
        auto node = heap[origPos];
        int downLeft = cur << 1;
        int downRight = downLeft + 1;
        if (downRight <= size && lessThan(heap[downRight], heap[downLeft])) {
            downLeft = downRight;
        }
        while (downLeft <= size && lessThan(heap[downLeft], node)) {
            heap[downLeft] = heap[cur];
            cur = downLeft;
            downLeft = cur << 1;
            downRight = downLeft + 1;
            if (downRight <= size && lessThan(heap[downRight], heap[downLeft])) {
                downLeft = downRight;
            }
        }
        heap[cur] = node;
        return true;
    }

    int DocPriorityQueue::getSize() {
        return size;
    }

    int DocPriorityQueue::getMaxSize() {
        return maxSize;
    }

    bool DocPriorityQueue::isEmpty() {
        return size == 0;
    }

    DocWithScore *DocPriorityQueue::pop() {
        if (size > 0) {
            auto result = new DocWithScore(heap[1]);
            heap[1] = heap[size];
            //clear it
            heap[size] = DocWithScore();
            size--;
            return result;
        } else {
            return nullptr;
        }
    }

    DocPriorityQueue::~DocPriorityQueue() {
        delete[] heap;
    }

    void DocPriorityQueue::clear() {
        size = 0;
        maxSize = 0;
        delete[] heap;
    }

    bool DocPriorityQueue::remove(const DocWithScore &element) {
        for (int i = 1; i < size; i++) {
            if (heap[i] == element) {
                heap[i] = heap[size];
                //clear it
                heap[size] = DocWithScore();
                size--;
                if (i <= size) {
                    if (!upHeap(i)) {
                        downHeap(i);
                    }
                }
                return true;
            }
        }
        return false;
    }
}