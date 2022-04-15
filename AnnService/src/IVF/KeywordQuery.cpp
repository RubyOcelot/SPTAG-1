#include "inc/IVF/KeywordQuery.h"

namespace IVF {

    std::unique_ptr<Scorer> KeywordQuery::getScorer() {
        return keyword->getScorer();
    }

    KeywordQuery::KeywordQuery(std::unique_ptr<Keyword> keyword, ScoreScheme *scoreScheme):keyword(std::move(keyword)),
    scoreScheme(scoreScheme)  {

    }

}