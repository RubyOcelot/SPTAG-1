#include "inc/IVF/utils/TokenStream.h"

namespace IVF{
    Token TokenStream::getToken() {
        return token;
    }

    void TokenStream::reset() {
        token="";
    }
}

