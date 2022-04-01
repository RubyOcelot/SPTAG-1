#ifndef IVF_VECTORSCORESCHEME_H
#define IVF_VECTORSCORESCHEME_H

#include "ScoreScheme.h"

namespace IVF {
    class VectorScoreScheme : public ScoreScheme {
    public:

        virtual void setQueryVector(void* queryVector) = 0;

        virtual VectorScoreScheme *clone() = 0;

        virtual void setVecLen(int len)=0;
        virtual int getVecLen()=0;

    };

}

#endif //IVF_VECTORSCORESCHEME_H
