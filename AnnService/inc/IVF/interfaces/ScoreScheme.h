#ifndef IVF_SCORESCHEME_H
#define IVF_SCORESCHEME_H

#include "../CommonUse.h"

namespace IVF {
    class ScoreScheme {
    public:
        virtual ScoreScheme *clone() = 0;

        virtual bool postingStatisticsLoader(std::istream *rawStream) = 0;

        virtual void keywordStatisticsLoader(std::string) = 0;

        virtual void collectionStatisticsLoader(std::string) = 0;

        virtual float score() = 0;

        virtual DocId getDocId() = 0;

        virtual ~ScoreScheme() = default;
    };

}


#endif //IVF_SCORESCHEME_H
