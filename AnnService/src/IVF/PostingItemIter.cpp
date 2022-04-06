#include "inc/IVF/PostingItemIter.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"

namespace IVF {
    ScoreScheme *PostingItemIter::getNext() {
        ScoreScheme *newItem = (modelItem->clone());

        if (newItem->postingStatisticsLoader(rawValue)) {
            return newItem;
        } else {
            delete newItem;
            return nullptr;
        }
    }

//TODO TESTC
    ScoreScheme *PostingItemIter::getFirst() {
        return getNext();
    }

    ScoreScheme *PostingItemIter::skipTo(DocId docId) {
        //TODO optimize
        auto newItem = getNext();
        while ((newItem != nullptr) && (newItem->getDocId() != -1) && (newItem->getDocId() < docId)) {
            newItem = getNext();
        }
        if (newItem->getDocId() == -1) {
            delete newItem;
            return nullptr;
        } else
            return newItem;
    }

    PostingItemIter::~PostingItemIter() {
        delete modelItem;
    }

    PostingItemIter::PostingItemIter(ScoreScheme *modelItem, std::istream *value) : modelItem(modelItem),
                                                                                    rawValue(value) {

    }

}