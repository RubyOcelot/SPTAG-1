#ifndef IVF_SUBSCORERSET_H
#define IVF_SUBSCORERSET_H

#include "../interfaces/Scorer.h"

namespace IVF {
    class SubScorerSet {
    public:
        void add(std::shared_ptr<Scorer> newScorer);

        std::list<std::shared_ptr<Scorer>> value;
    };
}

#endif //IVF_SUBSCORERSET_H
