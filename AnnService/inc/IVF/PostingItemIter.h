#ifndef IVF_POSTINGITEMITER_H
#define IVF_POSTINGITEMITER_H


#include "interfaces/ScoreScheme.h"

namespace IVF {
    class PostingItemIter {
    public:
        //TODO TESTC
        //PostingItemIter();
        PostingItemIter(std::unique_ptr<ScoreScheme> modelItem, std::istream *value);

        //TODO TESTC
        ScoreScheme *getFirst();

        ScoreScheme *getNext();

        ScoreScheme *skipTo(DocId docId);

        ~PostingItemIter();

    private:
        std::unique_ptr<ScoreScheme> modelItem;
        std::istream *rawValue;
    };
}

#endif //IVF_POSTINGITEMITER_H
