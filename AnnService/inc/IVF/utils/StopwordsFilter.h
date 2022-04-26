#ifndef SPTAGLIB_STOPWORDSFILTER_H
#define SPTAGLIB_STOPWORDSFILTER_H

#include "TokenFilter.h"
#include <set>

namespace IVF {
    class StopwordsFilter: public TokenFilter{
    public:
        explicit StopwordsFilter(std::unique_ptr<TokenStream> inputTokenStream,const std::string& wordFile);
        explicit StopwordsFilter(std::unique_ptr<TokenStream> inputTokenStream,std::unique_ptr<std::set<std::string>> stopwords);
        bool fetchNextToken() override;
        void close() override;
    private:
        std::unique_ptr<std::set<std::string>> stopwords;
    };
}

#endif //SPTAGLIB_STOPWORDSFILTER_H
