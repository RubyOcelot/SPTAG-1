#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include <memory>
#include "CommonUse.h"
#include "interfaces/Keyword.h"
#include "interfaces/Scorer.h"
#include "TermIndex.h"

namespace IVF {

    class TermIndex;
    class Term : public Keyword {
    public:
        Term(std::string str,std::shared_ptr<TermIndex> termIndex);

        Term();

        std::unique_ptr<Term> asFactory(const std::string &str);

        std::unique_ptr<Scorer> getScorer() override;
        int addToIndex() override;
        [[nodiscard]] std::string getStr() const;
        std::shared_ptr<TermIndex> termIndex;
    private:
        std::string str;
    };
}

#endif //SPTAGLIB_TERM_H
