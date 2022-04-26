#ifndef SPTAGLIB_ANALYZER_H
#define SPTAGLIB_ANALYZER_H

#include "TokenFilter.h"
#include "Tokenizer.h"

namespace IVF {

    class Analyzer {
    public:
        virtual std::unique_ptr<TokenStream> getTokenStream(std::unique_ptr<std::istream> inputStream)=0;
        virtual std::unique_ptr<TokenStream> getTokenStream(std::string inputString)=0;
        virtual ~Analyzer()=default;
    };

}
#endif //SPTAGLIB_ANALYZER_H
