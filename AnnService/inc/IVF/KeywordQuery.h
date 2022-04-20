#ifndef IVF_KEYWORDQUERY_H
#define IVF_KEYWORDQUERY_H

#include "interfaces/Query.h"
#include "interfaces/Keyword.h"

namespace IVF {
    class KeywordQuery : public Query {
    public:
        explicit KeywordQuery(std::unique_ptr<Keyword> keyword);

        std::unique_ptr<Keyword> keyword;

        std::unique_ptr<Scorer> getScorer() override;
    };
}

#endif //IVF_KEYWORDQUERY_H
