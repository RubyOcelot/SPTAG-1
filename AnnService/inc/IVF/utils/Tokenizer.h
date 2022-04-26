#ifndef SPTAGLIB_TOKENIZER_H
#define SPTAGLIB_TOKENIZER_H

#include <istream>

#include "TokenStream.h"

namespace IVF {
    class Tokenizer : public TokenStream {
    public:
        explicit Tokenizer(std::unique_ptr<std::istream> inputStream);


    protected:
        std::unique_ptr<std::istream> inputStream;
    };

}

#endif //SPTAGLIB_TOKENIZER_H
