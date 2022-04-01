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
        explicit KeyVector(void* vecValue);

        std::unique_ptr<Scorer> getScorer() override;

        std::unique_ptr<Scorer> getScorer(VectorScoreScheme *scoreScheme);

        //default: DefaultVectorScoreScheme
        static void setCollectionStatHolder(VectorScoreScheme *collectionStat);

        static VectorScoreScheme *getCollectionStatHolder();

        void* getQueryVector();

        static VectorScoreScheme *collectionStatHolder;

        static VectorIndexWrapper<T>* vectorIndexWrapper;
    private:
        void* queryVector;
    };


}

#endif //IVF_KEYVECTOR_H
