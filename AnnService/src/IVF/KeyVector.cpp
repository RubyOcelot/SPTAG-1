#include <utility>
#include "inc/Core/Common.h"
#include "inc/IVF/KeyVector.h"
#include "inc/IVF/SimpleScorer.h"
#include "inc/IVF/PostingItemIter.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include "inc/IVF/utils/SubScorerSet.h"
#include "inc/IVF/BooleanScorer.h"
#include "inc/IVF/VectorIndexWrapper.h"


namespace IVF {

    VectorScoreScheme *KeyVector::collectionStatHolder;

    std::unique_ptr<Scorer> KeyVector::getScorer() {
        auto defaultScoreScheme = KeyVector::getCollectionStatHolder();
        if (defaultScoreScheme == nullptr) {
            //TODO error;
        }
        return getScorer(defaultScoreScheme);
    }

//consume the pointer
    std::unique_ptr<Scorer> KeyVector::getScorer(VectorScoreScheme *vScoreScheme) {
        if (vScoreScheme == nullptr) {
            //TODO error;
        } else {
            vScoreScheme->setQueryVector(vectorValue);
        }


        std::vector<SizeType> headIDs;
        vectorIndexWrapper->getHeadIDs(headIDs, getQueryVector());

        //call storage
        auto subScorers = std::make_unique<SubScorerSet>();
        for (auto headID: headIDs) {
            //TODO load kwStat
            auto kwStatHolder = vScoreScheme->smart_clone();
            auto *postingItemStream=new std::istringstream;
            vectorIndexWrapper->getPostingList(headID, postingItemStream);
            //consume pointer
            auto *pIter = new PostingItemIter(std::move(kwStatHolder), postingItemStream);
            //consume pointer
            std::unique_ptr<Scorer> clusterScorer = std::make_unique<SimpleScorer>(*pIter);
            subScorers->add(std::move(clusterScorer));
        }

        //consume input pointer
        delete vScoreScheme;
        std::unique_ptr<Scorer> retScorer = std::make_unique<BooleanScorer>(LogicOperator::OR, std::move(subScorers));
        return retScorer;
    }


    void KeyVector::setCollectionStatHolder(VectorScoreScheme *collectionStat) {
        collectionStatHolder = collectionStat;
    }

    VectorScoreScheme *KeyVector::getCollectionStatHolder() {
        if (collectionStatHolder != nullptr) {
            return collectionStatHolder->clone();
        } else {
            return nullptr;
        }
    }


    void* KeyVector::getQueryVector() {
        return vectorValue;
    }

    int KeyVector::addToIndex() {
        vectorIndexWrapper->addVector(vectorValue, 1);
        //TODO vector lifetime
        return 0;
    }

    std::unique_ptr<KeyVector> KeyVector::asFactory(void *vecValue) {
        auto kv=std::make_unique<KeyVector>(vecValue,vectorIndexWrapper);
        return kv;
    }

    KeyVector::KeyVector(void *vecValue, std::shared_ptr<VectorIndexWrapper> vectorIndexWrapper) : vectorValue(vecValue), vectorIndexWrapper(std::move(vectorIndexWrapper)){

    }

    KeyVector::KeyVector():vectorValue(nullptr) {

    }


}
