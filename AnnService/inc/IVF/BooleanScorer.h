#ifndef IVF_BOOLEANSCORER_H
#define IVF_BOOLEANSCORER_H

#include "interfaces/Scorer.h"
#include "utils/SubScorerSet.h"
#include "utils/SizedPriorityQueue.h"

namespace IVF {
    class BooleanScorer : public Scorer {
    public:
        BooleanScorer(const LogicOperator &op, std::unique_ptr<SubScorerSet> in_subScorers);

        float score() override;

        DocId next() override;

        DocId skipTo(DocId targetId) override;

        DocId getCurrentId() override;
    private:
        LogicOperator op;
        std::unique_ptr<SubScorerSet> subScorers;
        std::unique_ptr<SizedPriorityQueue<Scorer>> pq;
        DocId curId;
    };
}

#endif //IVF_BOOLEANSCORER_H
