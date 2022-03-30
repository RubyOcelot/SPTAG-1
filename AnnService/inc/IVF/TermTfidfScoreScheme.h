#ifndef IVF_TERMTFIDFSCORESCHEME_H
#define IVF_TERMTFIDFSCORESCHEME_H

#include "interfaces/ScoreScheme.h"

namespace IVF {
//TODO term
    class TermTFIDFScoreScheme : public ScoreScheme {

        ScoreScheme *clone();

        bool postingStatisticsLoader(std::istream *rawStream);

        void keywordStatisticsLoader(std::string);

        void collectionStatisticsLoader(std::string);

        float score();

        DocId getDocId();

    };
}

#endif //IVF_TERMTFIDFSCORESCHEME_H
