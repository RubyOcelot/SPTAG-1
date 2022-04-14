#ifndef IVF_TERMTFIDFSCORESCHEME_H
#define IVF_TERMTFIDFSCORESCHEME_H

#include <math.h>
#include <atomic>
#include <string>

#include "interfaces/ScoreScheme.h"
#include "inc/Core/Common.h"
#include "inc/IVF/utils/ByteToDocId.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"

namespace IVF {
    typedef uint32_t TermStatType;

    class TermTFIDFScoreScheme : public ScoreScheme {
        class DocFreq:public KeywordStatistic{
        public:
            DocFreq();
            void reset()override;
            void set(const std::string &)override;
            void modForAdd(const std::string &)override;
            void modForDel(const std::string &)override;
            std::string getContent()override;
            std::unique_ptr<KeywordStatistic> clone()override;
            std::unique_ptr<KeywordStatistic> getNew()override;
        private:
            std::atomic<int> docFreq;
        };
    public:
        ScoreScheme *clone();

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;

        std::unique_ptr<KeywordStatistic> getKeywordStatistic();
    private:
        float termFreq;
        TermStatType docFreq;
        TermStatType docNum;
        DocId docId = -1;
    };

}

#endif //IVF_TERMTFIDFSCORESCHEME_H
