#include <utility>
#include "inc/IVF/KeyVector.h"
#include "inc/IVF/SimpleScorer.h"
#include "inc/IVF/PostingItemIter.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include "inc/IVF/utils/SubScorerSet.h"
#include "inc/IVF/BooleanScorer.h"
#include "inc/IVF/SPTAGWarpper.h"
#include "inc/IVF/VectorIndexWrapper.h"


namespace IVF {

    template<class T>
    VectorScoreScheme *KeyVector<T>::collectionStatHolder;
    template<class T>
    VectorIndexWrapper<T> *KeyVector<T>::vectorIndexWrapper;

    template<class T>
    std::unique_ptr<Scorer> KeyVector<T>::getScorer() {
        auto defaultScoreScheme = KeyVector::getCollectionStatHolder();
        if (defaultScoreScheme == nullptr) {
            //TODO error;
        }
        return getScorer(defaultScoreScheme);
    }

//consume the pointer
    template<class T>
    std::unique_ptr<Scorer> KeyVector<T>::getScorer(VectorScoreScheme *vScoreScheme) {
        if (vScoreScheme == nullptr) {
            //TODO error;
        } else {
            vScoreScheme->setQueryVector(queryVector);
        }


        std::vector<int> headIDs;
        vectorIndexWrapper->getHeadIDs(headIDs, getQueryVector());

        //call storage
        auto subScorers = std::make_unique<SubScorerSet>();
        for (auto headID: headIDs) {
            //TODO load kwStat
            auto kwStatHolder = vScoreScheme->clone();
            auto *postingItemStream=new std::istringstream;
            vectorIndexWrapper->getPostingList(headID, postingItemStream);
            //consume pointer
            auto *pIter = new PostingItemIter(kwStatHolder, postingItemStream);
            //consume pointer
            std::unique_ptr<Scorer> clusterScorer = std::make_unique<SimpleScorer>(*pIter);
            subScorers->add(std::move(clusterScorer));
        }


        //consume input pointer
        delete vScoreScheme;
        std::unique_ptr<Scorer> retScorer = std::make_unique<BooleanScorer>(LogicOperator::OR, std::move(subScorers));
        return retScorer;
    }

    template<class T>
    KeyVector<T>::KeyVector(void* vecValue) : queryVector(vecValue) {

    }

    template<class T>
    void KeyVector<T>::setCollectionStatHolder(VectorScoreScheme *collectionStat) {
        collectionStatHolder = collectionStat;
    }

    template<class T>
    VectorScoreScheme *KeyVector<T>::getCollectionStatHolder() {
        if (collectionStatHolder != nullptr) {
            return collectionStatHolder->clone();
        } else {
            return nullptr;
        }
    }


    template<class T>
    void* KeyVector<T>::getQueryVector() {
        return queryVector;
    }

    template class KeyVector<float>;
    template class KeyVector<int8_t>;
}
