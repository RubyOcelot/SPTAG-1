#ifndef IVF_DEFAULTVECTORSCORESCHEME_H
#define IVF_DEFAULTVECTORSCORESCHEME_H

#include "interfaces/ScoreScheme.h"
#include "utils/DistanceFunction.h"
#include "interfaces/VectorScoreScheme.h"


namespace IVF {

    template<class T>
    class DefaultVectorScoreScheme : public VectorScoreScheme<T> {
    public:
        //copy constructor
        DefaultVectorScoreScheme(const DefaultVectorScoreScheme& d);
        void setQueryVector(std::shared_ptr<std::vector<T>> qVector) override;

        VectorScoreScheme<T> *clone() override;

//    ScoreScheme* clone() override;
        explicit DefaultVectorScoreScheme(std::shared_ptr<DistanceFunction<T>> distFunc=std::make_unique<L2DistanceFunction<T>>(), std::shared_ptr<std::vector<T>> queryVector=nullptr, DocId docId=-1, std::shared_ptr<std::vector<T>> docVector=nullptr,  int vecLen=-1);

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;

        ~DefaultVectorScoreScheme() override;

        void setVecLen(int len)override;
        int getVecLen()override;

    private:

        DocId docId = -1;
        std::shared_ptr<std::vector<T>> docVector;
        std::shared_ptr<std::vector<T>> queryVector;
        //default L2DistanceFunction
        std::shared_ptr<DistanceFunction<T>> distFunc;
        int vecLen=-1;
        //int docNum;

    };

    template class VectorScoreSchemeFactory<DefaultVectorScoreScheme<float>,float>;
    template class VectorScoreSchemeFactory<DefaultVectorScoreScheme<int8_t>,int8_t>;
}

#endif //IVF_DEFAULTVECTORSCORESCHEME_H
