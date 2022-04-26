#include <sstream>
#include <utility>

#include "inc/IVF/utils/EnglishSimpleAnalyzer.h"
#include "inc/IVF/utils/WhiteSpaceTokenizer.h"
#include "inc/IVF/utils/StopwordsFilter.h"
#include "inc/IVF/utils/EnglishMinimalStemFilter.h"
#include "inc/IVF/utils/LowerCaseFilter.h"

namespace IVF{

    std::unique_ptr<TokenStream> EnglishSimpleAnalyzer::getTokenStream(std::string inputString) {
        return getTokenStream(std::make_unique<std::istringstream>(inputString));
    }

    std::unique_ptr<TokenStream> EnglishSimpleAnalyzer::getTokenStream(std::unique_ptr<std::istream> inputStream) {

        auto whiteSpace=std::make_unique<WhiteSpaceTokenizer>(std::move(inputStream));
        auto lowerCase=std::make_unique<LowerCaseFilter>(std::move(whiteSpace));
        std::unique_ptr<TokenStream> stopwords;
        if(!stopwordsFile.empty()) {
            stopwords = std::make_unique<StopwordsFilter>(std::move(lowerCase), stopwordsFile);
        }
        else{
            stopwords = std::make_unique<StopwordsFilter>(std::move(lowerCase), std::make_unique<std::set<std::string>>(stopwordsSet));
        }
        auto stemResult=std::make_unique<EnglishMinimalStemFilter>(std::move(stopwords));
        return stemResult;
    }

    EnglishSimpleAnalyzer::EnglishSimpleAnalyzer(std::string stopwordsFile):stopwordsFile(std::move(stopwordsFile)) {

    }
}