#ifndef SPTAGLIB_BASICTOKENSTREAM_H
#define SPTAGLIB_BASICTOKENSTREAM_H

#include <vector>
#include "TokenStream.h"

namespace IVF {

    class SimpleTokenStream : public TokenStream {
    public:
        explicit SimpleTokenStream(std::unique_ptr<std::vector<Token>> inputTokens);

        bool fetchNextToken() override;

        void close() override;

        void end() override;

        void reset() override;

    private:
        std::unique_ptr<std::vector<Token>> inputTokens;
        int curPos;
    };
}


#endif //SPTAGLIB_BASICTOKENSTREAM_H
