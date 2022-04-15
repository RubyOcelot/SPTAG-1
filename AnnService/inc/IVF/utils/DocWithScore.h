#ifndef IVF_DOCWITHSCORE_H
#define IVF_DOCWITHSCORE_H

#include "../CommonUse.h"

namespace IVF {
    class DocWithScore {
    public:
        DocId docId;
        float score;

        void print() const;

        bool operator==(const DocWithScore &a) const;

        DocWithScore();

        DocWithScore(DocId docId, float score);
    };
}


#endif //IVF_DOCWITHSCORE_H
