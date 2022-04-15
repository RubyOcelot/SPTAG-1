#ifndef IVF_KEYVECTOR_H
#define IVF_KEYVECTOR_H

#include "CommonUse.h"
#include "interfaces/Keyword.h"
#include "interfaces/VectorScoreScheme.h"
#include "VectorIndexWrapper.h"

namespace IVF {

    class KeyVector : public Keyword {
    public:
        KeyVector(void* vecValue,std::shared_ptr<VectorIndexWrapper> vectorIndexWrapper);

        KeyVector();

        std::unique_ptr<KeyVector> asFactory(void* vecValue);

        std::unique_ptr<Scorer> getScorer() override;

        int addToIndex() override;

        std::unique_ptr<Scorer> getScorer(VectorScoreScheme *scoreScheme);

        void* getQueryVector();

        std::shared_ptr<VectorIndexWrapper> vectorIndexWrapper;

        //default: DefaultVectorScoreScheme
        static void setCollectionStatHolder(VectorScoreScheme *collectionStat);

        static VectorScoreScheme *getCollectionStatHolder();

        static VectorScoreScheme *collectionStatHolder;


    private:
        void* vectorValue;
    };


}

#endif //IVF_KEYVECTOR_H
