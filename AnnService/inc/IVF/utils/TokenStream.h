#ifndef SPTAGLIB_TOKENSTREAM_H
#define SPTAGLIB_TOKENSTREAM_H

#include <string>
#include <memory>

namespace IVF {

    typedef std::string Token;

    class TokenStream {
    public:
        virtual bool fetchNextToken()=0;
        Token getToken();
        virtual void close()=0;
        virtual void end()=0;
        virtual void reset();
        virtual ~TokenStream()=default;
    protected:
        Token token;
    };

}

#endif //SPTAGLIB_TOKENSTREAM_H
