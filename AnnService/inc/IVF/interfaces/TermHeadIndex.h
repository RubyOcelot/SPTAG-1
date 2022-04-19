#ifndef SPTAGLIB_TERMHEADINDEX_H
#define SPTAGLIB_TERMHEADINDEX_H

#include "inc/IVF/CommonUse.h"
#include "inc/IVF/Term.h"

namespace IVF {

    class TermHeadIndex {
    public:
        virtual HeadIDType seek(const Term &term, std::string *rt_stat)=0;

        virtual HeadIDType set(const Term &term, std::string stat)=0;

        virtual HeadIDType add(const Term &term, std::string inputStat)=0;

        virtual HeadIDType del(const Term &term, std::string inputStat)=0;
    };
}

#endif //SPTAGLIB_TERMHEADINDEX_H
