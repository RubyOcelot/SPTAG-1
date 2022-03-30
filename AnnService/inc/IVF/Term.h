#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include "ForwardDefine.h"
#include "interfaces/Keyword.h"

namespace IVF {

    class Term : public Keyword {

        std::unique_ptr<Scorer> getScorer() override;
    };
}

#endif //SPTAGLIB_TERM_H
