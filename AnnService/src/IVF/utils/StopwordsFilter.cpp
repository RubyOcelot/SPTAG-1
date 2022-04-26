#include <utility>
#include <fstream>

#include "inc/IVF/utils/StopwordsFilter.h"

namespace IVF {

    IVF::StopwordsFilter::StopwordsFilter(std::unique_ptr<TokenStream> inputTokenStream,
                                          std::unique_ptr<std::set<std::string>> stopwords) : stopwords(
            std::move(stopwords)),
                                                                                              TokenFilter(std::move(
                                                                                                      inputTokenStream)) {

    }

    IVF::StopwordsFilter::StopwordsFilter(std::unique_ptr<TokenStream> inputTokenStream, const std::string &wordFile)
            : TokenFilter(std::move(inputTokenStream)), stopwords(std::make_unique<std::set<std::string>>()) {
        std::fstream fs;
        fs.open(wordFile);
        while (fs) {
            std::string word;
            fs >> word;
            stopwords->insert(word);
        }

    }

    bool IVF::StopwordsFilter::fetchNextToken() {
        token="";
        while(inputTokenStream->fetchNextToken()){
            if(stopwords->find(inputTokenStream->getToken())!=stopwords->end()){
                continue;
            }
            token=inputTokenStream->getToken();
            return true;
        }
        return false;
    }

    void StopwordsFilter::close() {
        inputTokenStream->close();
        inputTokenStream.reset();
        stopwords.reset();
    }


}