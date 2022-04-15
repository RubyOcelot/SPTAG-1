#include "inc/IVF/Term.h"

#include <utility>

namespace IVF {

    std::unique_ptr<Scorer> IVF::Term::getScorer() {
        auto termScoreScheme=Term::getCollectionStatHolder();
        if (termScoreScheme == nullptr) {
            //TODO error;
        }
        return nullptr;
    }

    std::string Term::getStr() const {
        return str;
    }

    Term::Term(std::string str, std::shared_ptr<TermIndex> termIndex):str(std::move(str)), termIndex(std::move(termIndex)) {

    }

    int Term::addToIndex() {
        return 0;
    }

    ScoreScheme *Term::getCollectionStatHolder() {
        return nullptr;
    }

    Term::Term() = default;

}