#include "inc/IVF/utils/DocPriorityQueue.h"


namespace IVF {
    template<class T>
    DocPriorityQueue<T>::DocPriorityQueue(int maxSize,bool (*lessThanFunc)(const std::shared_ptr<T> &a,const std::shared_ptr<T> &b)) : maxSize(maxSize),lessThanFunc(lessThanFunc) {
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
        heap = std::make_unique<std::shared_ptr<T>[]>(heapSize) ;
    }

//    template<class T>
//    float DocPriorityQueue<T>::getMinScore() {
//        return top()->score;
//    }

    template<class T>
    std::shared_ptr<T> DocPriorityQueue<T>::top() {
        //TODO isEmpty?
        //TODO return copy?
        return heap[1];
    }

    template<class T>
    std::shared_ptr<T> DocPriorityQueue<T>::add(const std::shared_ptr<T> &element) {
        int index = size + 1;
        heap[index] = element;
        size = index;
        upHeap(index);
        return heap[1];
    }


    template<class T>
    bool DocPriorityQueue<T>::lessThan(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) {
        return (*lessThanFunc)(a,b);
    }

// return dropped element if heap is full
    template<class T>
    std::shared_ptr<T> DocPriorityQueue<T>::insertWithOverflow(const std::shared_ptr<T> &element) {
        if (size < maxSize) {
            add(element);
            return nullptr;
        } else {
            if (size > 0 && lessThan(heap[1], element)) {
                auto ret = heap[1];
                heap[1] = element;
                updateTop();
                return ret;
            } else {
                return element;
            }
        }
    }

    template<class T>
    std::shared_ptr<T> DocPriorityQueue<T>::updateTop() {
        downHeap(1);
        return heap[1];
    }

    template<class T>
    bool DocPriorityQueue<T>::upHeap(int origPos) {
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

    template<class T>
    bool DocPriorityQueue<T>::downHeap(int origPos) {
        int cur = origPos;
        auto node = heap[origPos];
        int downLeft = cur << 1;
        int downRight = downLeft + 1;
        if (downRight <= size && lessThan(heap[downRight], heap[downLeft])) {
            downLeft = downRight;
        }
        while (downLeft <= size && lessThan(heap[downLeft], node)) {
            heap[cur] = heap[downLeft];
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

    template<class T>
    int DocPriorityQueue<T>::getSize() {
        return size;
    }

    template<class T>
    int DocPriorityQueue<T>::getMaxSize() {
        return maxSize;
    }

    template<class T>
    bool DocPriorityQueue<T>::isEmpty() {
        return size == 0;
    }

    template<class T>
    std::shared_ptr<T> DocPriorityQueue<T>::pop() {
        if (size > 0) {
            auto result = heap[1];
            heap[1] = heap[size];
            heap[size] = nullptr;
            size--;
            updateTop();
            return result;
        } else {
            return nullptr;
        }
    }


    template<class T>
    void DocPriorityQueue<T>::clear() {
        size = 0;
        maxSize = 0;
        heap.reset();
    }

    template<class T>
    bool DocPriorityQueue<T>::remove(const std::shared_ptr<T> &element) {
        for (int i = 1; i <= size; i++) {
            if (heap[i] == element) {
                heap[i] = heap[size];
                //clear it
                heap[size] = nullptr;
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

    template<class T>
    void DocPriorityQueue<T>::debug_print() {

        for (int i = 1; i <= size; i++) {
            heap[i]->print();
        }
        std::cout<<std::endl;
    }
}

template class IVF::DocPriorityQueue<IVF::DocWithScore>;