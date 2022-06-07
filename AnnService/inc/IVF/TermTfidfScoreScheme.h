#ifndef IVF_TERMTFIDFSCORESCHEME_H
#define IVF_TERMTFIDFSCORESCHEME_H

#include <cmath>
#include <atomic>
#include <string>

#include "interfaces/ScoreScheme.h"
#include "inc/Core/Common.h"
#include "inc/IVF/utils/DataTransform.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/PostingStatistics.h"
#include "inc/IVF/utils/TermSetDataHolder.h"

namespace IVF {

    class TermTFIDFScoreScheme : public ScoreScheme {
    public:
        typedef uint32_t TermStatType;

        static std::string TermStatToString(const TermStatType& data){
            std::string s;
            for(auto i=0;i<sizeof(data);i++){
                s+=*((char*)(&data)+i);
            }
            return s;
        }

        static TermStatType* StringToTermStat(const std::string& data){
            return (TermStatType*)data.c_str();
        }

        class TermFreq:public PostingStatistics{
        public:
            TermFreq(DocId docId,float termFreq);
            void reset()override;
            void set(const std::string &)override;
            std::string getContent()override;
        private:
            DocId docId;
            float termFreq;
        };

        class DocFreq:public KeywordStatistic{
        public:
            DocFreq();
            explicit DocFreq(TermStatType docFreq);
            void reset()override;
            void set(const std::string &)override;
            void modForAdd(const std::string &)override;
            void modForDel(const std::string &)override;
            std::string getContent()override;
            std::unique_ptr<KeywordStatistic> clone()override;
            std::unique_ptr<KeywordStatistic> getNew()override;
        private:
            std::atomic<TermStatType> docFreq;
        };
        class DocNum:public CollectionStatistic{
        public:
            DocNum();
            explicit DocNum(TermStatType docNum);
            void reset()override;
            void set(const std::string &)override;
            void modForAdd(const std::string &)override;
            void modForDel(const std::string &)override;
            std::string getContent()override;
            std::unique_ptr<CollectionStatistic> clone()override;
        private:
            std::atomic<TermStatType> docNum;
        };
        ScoreScheme *clone() override;

        std::unique_ptr<ScoreScheme> smart_clone() override;

        TermTFIDFScoreScheme();
        TermTFIDFScoreScheme(float termFreq,TermStatType docFreq,TermStatType docNum,DocId docId);

        //{DocId id,float tf}
        bool postingStatisticsLoader(std::istream *rawStream) override;
        //{TermStatType df}
        void keywordStatisticsLoader(std::string) override;
        //{TermStatType docNum}
        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;

        static std::unique_ptr<TermSetDataHolder> getTermSetData(const std::string& path);

        std::unique_ptr<KeywordStatistic> getEmptyKeywordStatistic() override;

        std::unique_ptr<CollectionStatistic> getEmptyCollectionStatistic() override;

        static std::unique_ptr<KeywordStatistic> getKeywordStatistic();

        static std::string serializePItem(DocId docId, float termFreq);
    private:
        float termFreq;
        TermStatType docFreq;
        TermStatType docNum;
        DocId docId = -1;
    };

}

#endif //IVF_TERMTFIDFSCORESCHEME_H
