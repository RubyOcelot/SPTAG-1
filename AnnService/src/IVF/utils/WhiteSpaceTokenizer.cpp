#include "inc/IVF/utils/WhiteSpaceTokenizer.h"

#include <utility>

bool IVF::WhiteSpaceTokenizer::fetchNextToken() {
    char ch;
    Token nextToken;
    while(inputStream->get(ch)){
        if(!isspace(ch,std::locale::classic())){
            nextToken+=ch;
            break;
        }
    }
    if(nextToken.length()==0){
        return false;
    }
    while(inputStream->get(ch)){
        if(!isspace(ch,std::locale::classic())){
            nextToken+=ch;
        } else{
            break;
        }
    }
    token=nextToken;
    return true;
}

void IVF::WhiteSpaceTokenizer::close() {
    inputStream.reset();
}

void IVF::WhiteSpaceTokenizer::reset() {
    token="";
}

void IVF::WhiteSpaceTokenizer::end() {

}


