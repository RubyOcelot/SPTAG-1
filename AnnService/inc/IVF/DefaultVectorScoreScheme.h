#ifndef IVF_DEFAULTVECTORSCORESCHEME_H
#define IVF_DEFAULTVECTORSCORESCHEME_H

#include "interfaces/ScoreScheme.h"
#include "utils/DistanceFunction.h"
#include "interfaces/VectorScoreScheme.h"


namespace IVF {
    template<class T>
    class DefaultVectorScoreScheme : public VectorScoreScheme {
    public:
        //copy constructor
        DefaultVectorScoreScheme(const DefaultVectorScoreScheme& d);
        void setQueryVector(void* qVector) override;

        VectorScoreScheme *clone() override;

        std::unique_ptr<ScoreScheme> smart_clone() override;

        SPTAG::DistCalcMethod getDefaultDistCalcFunc() override;

        explicit DefaultVectorScoreScheme(std::shared_ptr<DistanceFunction> distFunc=std::make_unique<DistanceUtilsWrap<T>>(defaultDistCalcFunc), void* queryVector=nullptr, DocId docId=-1, void* docVector=nullptr, int vecLen=-1);

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;

        ~DefaultVectorScoreScheme() override;

        void setVecLen(int len)override;
        int getVecLen()override;
        std::unique_ptr<KeywordStatistic> getEmptyKeywordStatistic()override;
    private:

        static const SPTAG::DistCalcMethod defaultDistCalcFunc;

        DocId docId = -1;
        void* docVector;
        void* queryVector;
        //default DistanceUtilsWrap
        std::shared_ptr<DistanceFunction> distFunc;
        int vecLen=-1;
        //int docNum;

    };
    template <class T> const SPTAG::DistCalcMethod DefaultVectorScoreScheme<T>::defaultDistCalcFunc=SPTAG::DistCalcMethod::L2;

}

#endif //IVF_DEFAULTVECTORSCORESCHEME_H
