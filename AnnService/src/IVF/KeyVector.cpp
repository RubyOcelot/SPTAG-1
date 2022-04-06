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

    VectorScoreScheme *KeyVector::collectionStatHolder;

    VectorIndexWrapper *KeyVector::vectorIndexWrapper;

    std::unique_ptr<VectorScoreSchemeFactoryBase> KeyVector::withCollectionStat;

    std::unique_ptr<Scorer> KeyVector::getScorer() {
        if (KeyVector::withCollectionStat == nullptr) {
            //TODO error;
        }
        return getScorer();
    }

//consume the pointer
    std::unique_ptr<Scorer> KeyVector::getScorer(std::unique_ptr<VectorScoreSchemeFactoryBase> p_vFactory) {
        if (p_vScoreScheme == nullptr) {
            //TODO error;
        } else {
            p_vScoreScheme->setQueryVector(queryVector);
        }

        std::vector<int> headIDs;
        vectorIndexWrapper->getHeadIDs(headIDs, getQueryVector());

        //call storage
        auto subScorers = std::make_unique<SubScorerSet>();
        for (auto headID: headIDs) {
            //TODO load kwStat
            auto kwStatHolder = p_vScoreScheme->clone();
            auto *postingItemStream=new std::istringstream;
            vectorIndexWrapper->getPostingList(headID, postingItemStream);
            //consume pointer
            auto *pIter = new PostingItemIter(kwStatHolder, postingItemStream);
            //consume pointer
            std::unique_ptr<Scorer> clusterScorer = std::make_unique<SimpleScorer>(*pIter);
            subScorers->add(std::move(clusterScorer));
        }


        //consume input pointer
        delete p_vScoreScheme;
        std::unique_ptr<Scorer> retScorer = std::make_unique<BooleanScorer>(LogicOperator::OR, std::move(subScorers));
        return retScorer;
    }

    KeyVector::KeyVector(void* vecValue) : queryVector(vecValue) {

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
        return queryVector;
    }

}
