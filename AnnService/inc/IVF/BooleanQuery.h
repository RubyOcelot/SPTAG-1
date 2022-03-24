#ifndef IVF_BOOLEANQUERY_H
#define IVF_BOOLEANQUERY_H

#include "interfaces/Query.h"

namespace IVF {
    class BooleanQuery : public Query {
        LogicOperator op;
        std::list<Query *> subQuerys;

        std::unique_ptr<Scorer> getScorer() override;

    };
}

#endif //IVF_BOOLEANQUERY_H
