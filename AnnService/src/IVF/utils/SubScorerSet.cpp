#include "inc/IVF/utils/SubScorerSet.h"

namespace IVF {
    void SubScorerSet::add(std::shared_ptr<Scorer> newScorer) {
        value.push_back(std::move(newScorer));
    }
}