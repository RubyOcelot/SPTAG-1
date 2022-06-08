#include "inc/IVF/VecTermHybridScorer.h"

namespace IVF{

    float VecTermHybridScorer::score() {

        if(curId==-1)
            return -1.0;//TODO error
        float retScore=vecScorer->score()+termScorer->score();

        return retScore;
    }

    DocId VecTermHybridScorer::next() {
        while(vecScorer->getCurrentId()!=-1&&termScorer->getCurrentId()!=-1
            &&vecScorer->getCurrentId()!=termScorer->getCurrentId()){
            if(vecScorer->getCurrentId()<termScorer->getCurrentId()){
                vecScorer->skipTo(termScorer->getCurrentId());
            }
            else{
                termScorer->skipTo(vecScorer->getCurrentId());
            }
        }
        curId= (termScorer->getCurrentId()!=-1)?vecScorer->getCurrentId():-1;
        return curId;
    }

    DocId VecTermHybridScorer::skipTo(DocId targetId) {
        vecScorer->skipTo(targetId);
        termScorer->skipTo(targetId);
        while(vecScorer->getCurrentId()!=-1&&termScorer->getCurrentId()!=-1
              &&vecScorer->getCurrentId()!=termScorer->getCurrentId()){
            if(vecScorer->getCurrentId()<termScorer->getCurrentId()){
                vecScorer->skipTo(termScorer->getCurrentId());
            }
            else{
                termScorer->skipTo(vecScorer->getCurrentId());
            }
        }
        curId=(termScorer->getCurrentId()!=-1)?vecScorer->getCurrentId():-1;
        return curId;
    }

    DocId VecTermHybridScorer::getCurrentId() {
        return curId;
    }
}