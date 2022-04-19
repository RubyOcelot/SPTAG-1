#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include <memory>
#include "CommonUse.h"
#include "interfaces/Keyword.h"
#include "interfaces/Scorer.h"

namespace IVF {

    class TermIndex;
    class Term : public Keyword {
    public:
        Term(std::string str,std::shared_ptr<TermIndex> termIndex);

        Term();

        std::unique_ptr<Scorer> getScorer() override;
        int addToIndex() override;
        std::string getStr() const;
        static ScoreScheme* getCollectionStatHolder();
        std::shared_ptr<TermIndex> termIndex;
    private:
        std::string str;
    };
}

#endif //SPTAGLIB_TERM_H
