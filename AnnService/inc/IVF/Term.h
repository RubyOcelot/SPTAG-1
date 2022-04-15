#ifndef SPTAGLIB_TERM_H
#define SPTAGLIB_TERM_H

#include <memory>
#include "CommonUse.h"
#include "interfaces/Keyword.h"
#include "interfaces/Scorer.h"

namespace IVF {

    class Term : public Keyword {
    public:
        std::unique_ptr<Scorer> getScorer() override;
        static ScoreScheme* getCollectionStatHolder();
        std::string getStr() const;
    private:
        std::string str;
    };
}

#endif //SPTAGLIB_TERM_H
