#ifndef IVF_KEYVECTOR_H
#define IVF_KEYVECTOR_H

#include "ForwardDefine.h"
#include "interfaces/Keyword.h"
#include "interfaces/VectorScoreScheme.h"
#include "VectorIndexWrapper.h"
#include "inc/IVF/utils/VectorScoreSchemeFactory.h"

namespace IVF {

    class KeyVector : public Keyword {
    public:
        explicit KeyVector(void* vecValue);

        std::unique_ptr<Scorer> getScorer() override;

        std::unique_ptr<Scorer> getScorer(const std::shared_ptr<VectorScoreScheme>& p_vScoreScheme);

        //default: DefaultVectorScoreScheme
        static void setCollectionStatHolder(VectorScoreScheme *collectionStat);

        static VectorScoreScheme *getCollectionStatHolder();

        void* getQueryVector();

        static VectorScoreScheme *collectionStatHolder;
        static std::unique_ptr<VectorScoreSchemeFactoryBase> withCollectionStat;

        static VectorIndexWrapper* vectorIndexWrapper;
    private:
        void* queryVector;
    };


}

#endif //IVF_KEYVECTOR_H
