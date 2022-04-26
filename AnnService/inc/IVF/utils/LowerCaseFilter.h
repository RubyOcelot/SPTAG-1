#ifndef SPTAGLIB_LOWERCASEFILTER_H
#define SPTAGLIB_LOWERCASEFILTER_H

#include "TokenFilter.h"

namespace IVF {
    class LowerCaseFilter: public TokenFilter{
    public:
        using TokenFilter::TokenFilter;
        bool fetchNextToken() override;
        void close() override;
    };
}

#endif //SPTAGLIB_LOWERCASEFILTER_H
