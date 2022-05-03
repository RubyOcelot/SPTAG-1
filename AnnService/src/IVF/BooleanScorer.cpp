#include "inc/IVF/BooleanScorer.h"


namespace IVF {
    BooleanScorer::BooleanScorer(const LogicOperator &op, std::unique_ptr<SubScorerSet> in_subScorers) : op(op),
                                                                                                         subScorers(
                                                                                                                 std::move(
                                                                                                                         in_subScorers)), curId(-1) {
        if (op == LogicOperator::OR) {
            DocId minDocId = MAX_DOCID;
            for (const auto &iter: (subScorers)->value) {
                DocId id = iter->getCurrentId();
                if (id != -1 && id < minDocId) {
                    minDocId = id;
                }
            }
            if (minDocId == MAX_DOCID) {
                minDocId = -1;
            }
            curId=minDocId;
        } else {
            //TODO AND

        }
    }

    float BooleanScorer::score() {
        if(curId==-1)
            return -1.0;//TODO error
        float retScore=0;
        for (const auto &iter: (subScorers)->value) {
            DocId id = iter->getCurrentId();
            if(id==curId){
                retScore+=iter->score();
            }
        }
        return retScore;
    }

    DocId BooleanScorer::next() {

        DocId curDocId = getCurrentId();
        if (op == LogicOperator::OR) {
            DocId minDocId = MAX_DOCID;
            for (const auto &iter: (subScorers)->value) {
                DocId id = iter->getCurrentId();
                if (id != -1) {
                    if (id == curDocId) {
                        id = iter->next();
                    }
                    if (id != -1 && id < minDocId) {
                        minDocId = id;
                    }
                }
            }
            if (minDocId == MAX_DOCID) {
                minDocId = -1;
            }
            curId=minDocId;
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

    DocId BooleanScorer::getCurrentId() {
        return curId;
    }

////TODO impl
//std::unique_ptr<Scorer> BooleanScorer::clone() const {
//    return std::unique_ptr<Scorer>();
//}

}