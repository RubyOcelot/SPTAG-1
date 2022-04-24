#ifndef SPTAGLIB_LETTERTOKENIZER_H
#define SPTAGLIB_LETTERTOKENIZER_H

#include <locale>
#include "Tokenizer.h"

namespace IVF {

    class LetterTokenizer : public Tokenizer {
    public:
        bool fetchNextToken() override;

        void close() override;

        void end() override;

        void reset() override;
    };
}

#endif //SPTAGLIB_LETTERTOKENIZER_H
