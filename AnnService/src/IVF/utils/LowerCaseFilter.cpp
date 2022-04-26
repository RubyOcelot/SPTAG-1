#include <algorithm>
#include <cctype>
#include <string>

#include "inc/IVF/utils/LowerCaseFilter.h"

namespace IVF{

    void LowerCaseFilter::close() {

    }


    bool LowerCaseFilter::fetchNextToken() {
        token="";
        if(inputTokenStream->fetchNextToken()){
            token=inputTokenStream->getToken();
            std::transform(token.begin(),token.end(),token.begin(),[](unsigned char c){ return std::tolower(c); });
            return true;
        }
        return false;
    }

}