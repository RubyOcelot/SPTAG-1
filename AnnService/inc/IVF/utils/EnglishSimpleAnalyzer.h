#ifndef SPTAGLIB_ENGLISHSIMPLEANALYZER_H
#define SPTAGLIB_ENGLISHSIMPLEANALYZER_H

#include <set>
#include "Analyzer.h"

namespace IVF {

    class EnglishSimpleAnalyzer : public Analyzer {
    public:
        explicit EnglishSimpleAnalyzer(std::string stopwordsFile="");
        std::unique_ptr<TokenStream> getTokenStream(std::unique_ptr<std::istream> inputStream) override;
        std::unique_ptr<TokenStream> getTokenStream(std::string inputString) override;
    private:
        std::string stopwordsFile;
        const std::set<std::string> stopwordsSet {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};
    };
}


#endif //SPTAGLIB_ENGLISHSIMPLEANALYZER_H
