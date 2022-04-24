#ifndef SPTAGLIB_TOKENFILTER_H
#define SPTAGLIB_TOKENFILTER_H

#include <utility>

#include "TokenStream.h"

namespace IVF {
    class TokenFilter : public TokenStream {
    public:
        explicit TokenFilter(std::shared_ptr<TokenStream> inputTokenStream);

    protected:
        std::shared_ptr<TokenStream> inputTokenStream;

    };

    TokenFilter::TokenFilter(std::shared_ptr<TokenStream> inputTokenStream): inputTokenStream(std::move(inputTokenStream)) {

    }

}
#endif //SPTAGLIB_TOKENFILTER_H
