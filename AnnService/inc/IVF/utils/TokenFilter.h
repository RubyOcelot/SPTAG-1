#ifndef SPTAGLIB_TOKENFILTER_H
#define SPTAGLIB_TOKENFILTER_H

#include <utility>

#include "TokenStream.h"

namespace IVF {
    class TokenFilter : public TokenStream {
    public:
        explicit TokenFilter(std::unique_ptr<TokenStream> inputTokenStream);

    protected:
        std::unique_ptr<TokenStream> inputTokenStream;

    };


}
#endif //SPTAGLIB_TOKENFILTER_H
