#include "inc/IVF/interfaces/Scorer.h"

namespace IVF {
//TODO life cycle
    PostingItem *Scorer::getCurrentItem() {
        return currentItem;
    }

    DocId Scorer::getCurrentId() {
        if (currentItem != nullptr)
            return currentItem->getDocId();
        else return -1;
    }

    void Scorer::print() {
        std::cout<<getCurrentItem()<<" "<<score()<<std::endl;
    }
}