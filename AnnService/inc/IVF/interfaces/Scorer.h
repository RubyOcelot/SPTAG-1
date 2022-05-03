#ifndef IVF_SCORER_H
#define IVF_SCORER_H

#include "../CommonUse.h"
#include "ScoreScheme.h"

namespace IVF {
    class Scorer {
    public:
        virtual float score() = 0;

        virtual DocId next() = 0;

        virtual DocId skipTo(DocId id) = 0;

        PostingItem *getCurrentItem();

        virtual DocId getCurrentId();

    protected:
        //Or name it posting item?
        ScoreScheme *currentItem = nullptr;//TBM
    };

}


#endif //IVF_SCORER_H
