#include "inc/IVF/SimpleScorer.h"

namespace IVF {
    float SimpleScorer::score() {
        if (currentItem != nullptr)
            return currentItem->score();
        else return -1.0;//TODO
    }

    SimpleScorer::SimpleScorer(PostingItemIter &pIter) : pIter(pIter) {
        //TODO TESTC
        currentItem = pIter.getFirst();
    }

    DocId SimpleScorer::next() {
        currentItem = pIter.getNext();
        if (currentItem == nullptr)
            return -1;
        else return currentItem->getDocId();
    }

    DocId SimpleScorer::skipTo(DocId id) {
        currentItem = pIter.skipTo(id);
        if (currentItem == nullptr)
            return -1;
        else return currentItem->getDocId();
    }

////TODO
//std::unique_ptr<Scorer> SimpleScorer::clone() const {
//    return std::unique_ptr<Scorer>();
//}

}