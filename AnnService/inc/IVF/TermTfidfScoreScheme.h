#ifndef IVF_TERMTFIDFSCORESCHEME_H
#define IVF_TERMTFIDFSCORESCHEME_H

#include <math.h>

#include "interfaces/ScoreScheme.h"
#include "inc/Core/Common.h"
#include "inc/IVF/utils/ByteToDocId.h"

namespace IVF {
    typedef uint32_t TermStatType;

    class TermTFIDFScoreScheme : public ScoreScheme {
    public:
        ScoreScheme *clone();

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;
    private:
        float termFreq;
        TermStatType docFreq;
        TermStatType docNum;
        DocId docId = -1;
    };

}

#endif //IVF_TERMTFIDFSCORESCHEME_H
