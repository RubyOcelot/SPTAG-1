#ifndef IVF_DEFAULTVECTORSCORESCHEME_H
#define IVF_DEFAULTVECTORSCORESCHEME_H

#include "interfaces/ScoreScheme.h"
#include "utils/DistanceFunction.h"
#include "interfaces/VectorScoreScheme.h"
#include "inc/Core/Common.h"


namespace IVF {
    template<class T>
    class DefaultVectorScoreScheme : public VectorScoreScheme {
    public:
        //copy constructor
        DefaultVectorScoreScheme(const DefaultVectorScoreScheme& d);
        void setQueryVector(void* qVector) override;

        VectorScoreScheme *clone() override;

//    ScoreScheme* clone() override;
        explicit DefaultVectorScoreScheme(std::shared_ptr<DistanceFunction> distFunc=std::make_unique<DistanceUtilsWrap<T>>(SPTAG::DistCalcMethod::L2), void* queryVector=nullptr, DocId docId=-1, void* docVector=nullptr, int vecLen=-1);

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;

        ~DefaultVectorScoreScheme() override;

        void setVecLen(int len)override;
        int getVecLen()override;

    private:

        DocId docId;
        void* docVector;
        void* queryVector;
        //default DistanceUtilsWrap
        std::shared_ptr<DistanceFunction> distFunc;
        SPTAG::DimensionType vecLen;
        //int docNum;

    };
}

#endif //IVF_DEFAULTVECTORSCORESCHEME_H
