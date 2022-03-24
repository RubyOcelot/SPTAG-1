#include "inc/IVF/BooleanQuery.h"
#include "inc/IVF/utils/SubScorerSet.h"
#include "inc/IVF/BooleanScorer.h"

namespace IVF {
    std::unique_ptr<Scorer> BooleanQuery::getScorer() {
        auto subScorers = std::make_unique<SubScorerSet>();
        for (auto q: subQuerys) {
            subScorers->add(q->getScorer());
        }
        return std::make_unique<BooleanScorer>(op, std::move(subScorers));
    }
}