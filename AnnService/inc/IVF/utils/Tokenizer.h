#ifndef SPTAGLIB_TOKENIZER_H
#define SPTAGLIB_TOKENIZER_H

#include <istream>

#include "TokenStream.h"

namespace IVF {
    class Tokenizer : public TokenStream {
    public:
        explicit Tokenizer(std::shared_ptr<std::istream> inputStream);


    protected:
        std::shared_ptr<std::istream> inputStream;
    };

    Tokenizer::Tokenizer(std::shared_ptr<std::istream> inputStream):inputStream(std::move(inputStream)) {

    }

}

#endif //SPTAGLIB_TOKENIZER_H
