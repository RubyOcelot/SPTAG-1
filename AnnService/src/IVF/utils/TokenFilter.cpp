#include "inc/IVF/utils/TokenFilter.h"

namespace IVF{

    TokenFilter::TokenFilter(std::unique_ptr<TokenStream> inputTokenStream): inputTokenStream(std::move(inputTokenStream)) {

    }

    void TokenFilter::end() {
        inputTokenStream->end();
    }

}
