#ifndef IVF_SCORESCHEME_H
#define IVF_SCORESCHEME_H

#include "../CommonUse.h"
#include "KeywordStatistic.h"

namespace IVF {
    class ScoreScheme {
    public:
        virtual ScoreScheme *clone() = 0;

        virtual std::unique_ptr<ScoreScheme> smart_clone()=0;

        virtual bool postingStatisticsLoader(std::istream *rawStream) = 0;

        virtual void keywordStatisticsLoader(std::string) = 0;

        virtual void collectionStatisticsLoader(std::string) = 0;

        virtual float score() = 0;

        virtual DocId getDocId() = 0;

        virtual ~ScoreScheme() = default;

        virtual std::unique_ptr<KeywordStatistic> getEmptyKeywordStatistic()=0;
    };

}


#endif //IVF_SCORESCHEME_H
