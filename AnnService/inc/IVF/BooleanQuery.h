#ifndef IVF_BOOLEANQUERY_H
#define IVF_BOOLEANQUERY_H

#include "interfaces/Query.h"

namespace IVF {
    class BooleanQuery : public Query {
    public:
        BooleanQuery();
        explicit BooleanQuery(LogicOperator op);
        BooleanQuery(LogicOperator op, std::unique_ptr<std::vector<std::shared_ptr<Query>>> subQuerys);
        LogicOperator op;
        std::unique_ptr<std::vector<std::shared_ptr<Query>>> subQuerys;

        std::unique_ptr<Scorer> getScorer() override;

    };
}

#endif //IVF_BOOLEANQUERY_H
