#include "inc/IVF/KeywordQuery.h"

namespace IVF {
//KeywordQuery::KeywordQuery(Keyword &keyword):keyword(keyword) {
//
//}

    std::unique_ptr<Scorer> KeywordQuery::getScorer() {
        return keyword.getScorer();
    }

    KeywordQuery::KeywordQuery(Keyword &keyword, ScoreScheme *scoreScheme) : keyword(keyword),
                                                                             scoreScheme(scoreScheme) {

    };
}