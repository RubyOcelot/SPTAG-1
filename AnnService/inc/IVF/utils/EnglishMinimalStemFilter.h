#ifndef SPTAGLIB_ENGLISHMINIMALSTEMFILTER_H
#define SPTAGLIB_ENGLISHMINIMALSTEMFILTER_H

#include "TokenFilter.h"

namespace IVF {

    class EnglishMinimalStemFilter : public TokenFilter{
    public:
        using TokenFilter::TokenFilter;
        bool fetchNextToken() override;
        void close() override;
        void end() override;
        void reset() override;
    };

}


#endif //SPTAGLIB_ENGLISHMINIMALSTEMFILTER_H
