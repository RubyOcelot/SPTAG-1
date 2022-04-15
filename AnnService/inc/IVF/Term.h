#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include <memory>
#include "CommonUse.h"
#include "interfaces/Keyword.h"
#include "interfaces/Scorer.h"
#include "TermIndex.h"

namespace IVF {

    class Term : public Keyword {
    public:
        Term(std::string str,std::shared_ptr<TermIndex> termIndex);
        std::unique_ptr<Scorer> getScorer() override;
        int addToIndex() override;
        std::string getStr() const;
        static ScoreScheme* getCollectionStatHolder();
    private:
        std::string str;
        std::shared_ptr<TermIndex> termIndex;
    };
}

#endif //SPTAGLIB_TERM_H
