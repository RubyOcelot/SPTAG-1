#ifndef SPTAGLIB_STRINGHEADINDEX_H
#define SPTAGLIB_STRINGHEADINDEX_H

#include "inc/IVF/CommonUse.h"
#include "inc/IVF/Term.h"

namespace IVF {

    class StringHeadIndex {
    public:
        virtual HeadIDType seek(const std::string &str, std::string *rt_stat)=0;

        virtual HeadIDType set(const std::string &str, std::string stat)=0;

        virtual HeadIDType add(const std::string &str, std::string inputStat)=0;

        virtual HeadIDType del(const std::string &str, std::string inputStat)=0;
    };
}

#endif //SPTAGLIB_STRINGHEADINDEX_H
