#include "inc/IVF/BooleanScorer.h"


namespace IVF {
    bool BooleanScorerLessThanFunc(const std::shared_ptr<Scorer> &a, const std::shared_ptr<Scorer> &b){
        return a->getCurrentId()<b->getCurrentId();
    }
    BooleanScorer::BooleanScorer(const LogicOperator &op, std::unique_ptr<SubScorerSet> in_subScorers) :
        op(op), subScorers(std::move(in_subScorers)), curId(-1),
        pq(std::make_unique<SizedPriorityQueue<Scorer>>(subScorers->value.size(),BooleanScorerLessThanFunc)) {

        for(const auto& iter:subScorers->value){
            if(iter->getCurrentId()!=-1)
                pq->insertWithOverflow(iter);
        }

        if (op == LogicOperator::OR || op == LogicOperator::WAND ) {
            if(pq->isEmpty())
                curId=-1;
            else
                curId=pq->top()->getCurrentId();
        } else {
            //TODO AND

        }
    }

    float BooleanScorer::score() {
        if(curId==-1)
            return -1.0;//TODO error
        float retScore=0;

        if (op == LogicOperator::OR) {
            retScore=pq->top()->score();
        }
        else if (op == LogicOperator::WAND) {
            for(const auto& iter:subScorers->value){
                if(iter->getCurrentId()==pq->top()->getCurrentId())
                    retScore+=iter->score();
            }
        }
        else{
            //TODO AND

        }
        return retScore;
    }


    DocId BooleanScorer::next() {
        if (op == LogicOperator::OR || op == LogicOperator::WAND ) {
            while(!pq->isEmpty()&&pq->top()->getCurrentId()==curId){
                if(pq->top()->next()==-1){
                    pq->removeTop();
                }
                else{
                    pq->updateTop();
                }
            }
            if(pq->isEmpty())
                curId=-1;
            else{
                curId=pq->top()->getCurrentId();
            }
        } else {
            //TODO AND

        }
        return curId;
    }

    DocId BooleanScorer::skipTo(DocId targetId) {
        if (op == LogicOperator::OR || op == LogicOperator::WAND ) {
            while (!pq->isEmpty()&& (pq->top()->getCurrentId()<targetId)){
                if(pq->top()->skipTo(targetId)==-1){
                    pq->removeTop();
                }
                else{
                    pq->updateTop();
                }
            }
            if(pq->isEmpty())
                curId=-1;
            else{
                curId=pq->top()->getCurrentId();
            }
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