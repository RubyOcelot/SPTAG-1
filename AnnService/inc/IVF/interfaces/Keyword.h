#ifndef IVF_KEYWORD_H
#define IVF_KEYWORD_H

#include "Keyword.h"
#include "Scorer.h"

namespace IVF {
    class Keyword {
    public:
        virtual std::unique_ptr<Scorer> getScorer() = 0;
//    virtual std::unique_ptr<Scorer> getScorer(ScoreScheme* scoreScheme)=0;
    };
}

#endif //IVF_KEYWORD_H
