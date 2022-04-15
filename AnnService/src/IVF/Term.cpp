#include "inc/IVF/Term.h"

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

}