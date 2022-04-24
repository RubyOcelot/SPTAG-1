#ifndef SPTAGLIB_STOPWORDFILTER_H
#define SPTAGLIB_STOPWORDFILTER_H

#include "TokenFilter.h"
#include <set>

namespace IVF {
    class StopwordFilter: public TokenFilter{
    public:
        explicit StopwordFilter(std::string wordFile);
        explicit StopwordFilter(std::set<std::string>);
        bool fetchNextToken() override;
        void close() override;
        void end() override;
        void reset() override;
    };
}

#endif //SPTAGLIB_STOPWORDFILTER_H
