#ifndef IVF_BOOLEANSCORER_H
#define IVF_BOOLEANSCORER_H

#include "interfaces/Scorer.h"
#include "utils/SubScorerSet.h"

namespace IVF {
    class BooleanScorer : public Scorer {
    public:
        BooleanScorer(const LogicOperator &op, std::unique_ptr<SubScorerSet> in_subScorers);

        float score() override;

        DocId next() override;

        DocId skipTo(DocId targetId) override;

    private:
        LogicOperator op;
        std::unique_ptr<SubScorerSet> subScorers;
    };
}

#endif //IVF_BOOLEANSCORER_H
