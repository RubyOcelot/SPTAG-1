#ifndef IVF_KEYVECTOR_H
#define IVF_KEYVECTOR_H

#include "ForwardDefine.h"
#include "interfaces/Keyword.h"
#include "interfaces/VectorScoreScheme.h"
#include "VectorIndexWrapper.h"

namespace IVF {

    template<class T>
    class KeyVector : public Keyword {
    public:
        explicit KeyVector(std::shared_ptr<std::vector<T>> vecValue);

        std::unique_ptr<Scorer> getScorer() override;

        std::unique_ptr<Scorer> getScorer(VectorScoreScheme<T> *scoreScheme);

        //default: DefaultVectorScoreScheme
        static void setCollectionStatHolder(VectorScoreScheme<T> *collectionStat);

        static VectorScoreScheme<T> *getCollectionStatHolder();

        std::shared_ptr<std::vector<T>> getQueryVector();

        static VectorScoreScheme<T> *collectionStatHolder;

        static VectorIndexWrapper<T>* vectorIndexWrapper;
    private:
        std::shared_ptr<std::vector<T>> queryVector;
    };


}

#endif //IVF_KEYVECTOR_H
