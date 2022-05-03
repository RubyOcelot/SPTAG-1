

#include "inc/IVF/utils/LetterTokenizer.h"

bool IVF::LetterTokenizer::fetchNextToken() {
    char ch;
    Token nextToken;
    while(inputStream->get(ch)){
        if(isalpha(ch,std::locale::classic())){
            nextToken+=ch;
            break;
        }
    }
    if(nextToken.length()==0){
        return false;
    }
    while(inputStream->get(ch)){
        if(isalpha(ch,std::locale::classic())){
            nextToken+=ch;
        } else{
            break;
        }
    }
    token=nextToken;
    return true;
}

void IVF::LetterTokenizer::close() {
    inputStream.reset();
}

void IVF::LetterTokenizer::end() {

}

void IVF::LetterTokenizer::reset() {
    token="";
}

