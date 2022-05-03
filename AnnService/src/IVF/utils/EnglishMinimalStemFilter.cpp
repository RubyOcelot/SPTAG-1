#include "inc/IVF/utils/EnglishMinimalStemFilter.h"

namespace IVF{


    bool EnglishMinimalStemFilter::fetchNextToken() {
        token="";
        if(inputTokenStream->fetchNextToken()){
            token=inputTokenStream->getToken();
            return true;
        }
        return false;
    }

    void EnglishMinimalStemFilter::close() {
        inputTokenStream->close();
        inputTokenStream.reset();
    }

    void EnglishMinimalStemFilter::end() {

    }

    void EnglishMinimalStemFilter::reset() {
        token="";
    }
}