#ifndef IVF_QUERY_H
#define IVF_QUERY_H

#include "Scorer.h"

namespace IVF {
    class Query {
    public:
        virtual std::unique_ptr<Scorer> getScorer() = 0;
//    Scorer& scorer;
    };
}

#endif //IVF_QUERY_H
