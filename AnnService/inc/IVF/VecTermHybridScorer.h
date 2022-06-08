#ifndef SPTAGLIB_VECTERMHYBRIDSCORER_H
#define SPTAGLIB_VECTERMHYBRIDSCORER_H

#include "interfaces/Scorer.h"

namespace IVF {
    class VecTermHybridScorer : public Scorer  {
    public:
        float score() override;

        DocId next() override;

        DocId skipTo(DocId targetId) override;

        DocId getCurrentId() override;
    private:
        std::unique_ptr<Scorer> vecScorer, termScorer;
        DocId curId;
    };
}

#endif //SPTAGLIB_VECTERMHYBRIDSCORER_H
