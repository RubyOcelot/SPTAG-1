#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include <memory>
#include "ForwardDefine.h"
#include "interfaces/Keyword.h"
#include "interfaces/Scorer.h"

namespace IVF {

    class Term : public Keyword {

        std::unique_ptr<Scorer> getScorer() override;
        static ScoreScheme* getCollectionStatHolder();
    };
}

#endif //SPTAGLIB_TERM_H
