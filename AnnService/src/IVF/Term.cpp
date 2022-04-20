#include <utility>

#include "inc/IVF/Term.h"
#include "inc/IVF/SimpleScorer.h"
#include "inc/IVF/PostingItemIter.h"


namespace IVF {

    std::unique_ptr<Scorer> IVF::Term::getScorer() {
        auto termScoreScheme=termIndex->getCollectionStatHolder();
        if (termScoreScheme == nullptr) {
            //TODO error;
        }
        std::string kwstat_str;
        auto hid=termIndex->getHeadID(this->getStr(),&kwstat_str);
        termScoreScheme->keywordStatisticsLoader(kwstat_str);

        std::istream **streamValue=new std::istream*;
        termIndex->getPostingList(hid, streamValue);
        //consume pointer
        auto *pIter = new PostingItemIter(std::move(termScoreScheme), *streamValue);
        delete streamValue;
        //consume pointer
        return std::make_unique<SimpleScorer>(*pIter);
    }

    std::string Term::getStr() const{
        return str;
    }

    Term::Term(std::string str, std::shared_ptr<TermIndex> termIndex):str(std::move(str)), termIndex(std::move(termIndex)) {

    }

    int Term::addToIndex() {
        return 0;
    }

    std::unique_ptr<Term> Term::asFactory(const std::string &str) {
        return std::make_unique<Term>(str,termIndex);
    }


    Term::Term() = default;

}