#include "inc/IVF/BooleanScorer.h"


namespace IVF {
    BooleanScorer::BooleanScorer(const LogicOperator &op, std::unique_ptr<SubScorerSet> in_subScorers) : op(op),
                                                                                                         subScorers(
                                                                                                                 std::move(
                                                                                                                         in_subScorers)) {
        if (op == LogicOperator::OR) {
            DocId minDocId = MAX_DOCID;
            for (const auto &iter: (subScorers)->value) {
                DocId id = iter->getCurrentId();
                if (id != -1 && id < minDocId) {
                    minDocId = id;
                    currentItem = iter->getCurrentItem();
                }
            }
            if (minDocId == MAX_DOCID) {
//            minDocId = -1;
                currentItem = nullptr;
            }
        } else {
            //TODO AND

        }
    }

    float BooleanScorer::score() {
        if (currentItem != nullptr)
            return currentItem->score();
        else return -1.0;//TODO
    }

    DocId BooleanScorer::next() {
        if (op == LogicOperator::OR) {
            DocId curDocId = getCurrentId();
            DocId minDocId = MAX_DOCID;
            for (const auto &iter: (subScorers)->value) {
                DocId id = iter->getCurrentId();
                if (id != -1) {
                    if (id == curDocId) {
                        id = iter->next();
                    }
                    if (id != -1 && id < minDocId) {
                        minDocId = id;
                        currentItem = iter->getCurrentItem();
                    }
                }
            }
            if (minDocId == MAX_DOCID) {
                minDocId = -1;
                currentItem = nullptr;
            }
            return minDocId;
        } else {
            //TODO AND

        }
        return -1;
    }

    DocId BooleanScorer::skipTo(DocId targetId) {
        if (op == LogicOperator::OR) {
            DocId minDocId = MAX_DOCID;
            for (const auto &iter: (subScorers)->value) {
                DocId id = iter->getCurrentId();
                while (id != -1 && id < targetId) {
                    id = iter->next();
                }
                if (id != -1 && id < minDocId) {
                    minDocId = id;
                    currentItem = iter->getCurrentItem();
                }
            }
            if (minDocId == MAX_DOCID) {
                minDocId = -1;
                currentItem = nullptr;
            }
            return minDocId;
        } else {
            //TODO AND

        }
        return -1;
    }

////TODO impl
//std::unique_ptr<Scorer> BooleanScorer::clone() const {
//    return std::unique_ptr<Scorer>();
//}

}