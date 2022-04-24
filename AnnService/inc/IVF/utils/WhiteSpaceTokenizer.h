#ifndef SPTAGLIB_WHITESPACETOKENIZER_H
#define SPTAGLIB_WHITESPACETOKENIZER_H

#include <cctype>
#include <locale>
#include "Tokenizer.h"

namespace IVF {

    class WhiteSpaceTokenizer: public Tokenizer {
    public:
        bool fetchNextToken() override;

        void close() override;

        void end() override;

        void reset() override;
    private:
    };

}
#endif //SPTAGLIB_WHITESPACETOKENIZER_H
