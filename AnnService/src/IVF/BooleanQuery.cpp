#include "inc/IVF/BooleanQuery.h"
#include "inc/IVF/utils/SubScorerSet.h"
#include "inc/IVF/BooleanScorer.h"

namespace IVF {
    std::unique_ptr<Scorer> BooleanQuery::getScorer() {
        auto subScorers = std::make_unique<SubScorerSet>();
        for (const auto& q: *subQuerys) {
            subScorers->add(q->getScorer());
        }
        return std::make_unique<BooleanScorer>(op, std::move(subScorers));
    }

    BooleanQuery::BooleanQuery() {

    }

    BooleanQuery::BooleanQuery(LogicOperator op):op(op) {

    }

    BooleanQuery::BooleanQuery(LogicOperator op, std::unique_ptr<std::vector<std::shared_ptr<Query>>> subQuerys):op(op),subQuerys(std::move(subQuerys)) {

    }
}