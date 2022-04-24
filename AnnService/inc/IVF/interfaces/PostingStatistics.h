#ifndef SPTAGLIB_POSTINGSTATISTICS_H
#define SPTAGLIB_POSTINGSTATISTICS_H

#include <string>
#include <memory>

namespace IVF {


    class PostingStatistics {
    public:
        virtual void reset() = 0;

        virtual void set(const std::string &) = 0;

        virtual std::string getContent() = 0;

    };

    class SortItem{
    public:
        std::string str;
        std::unique_ptr<PostingStatistics> postingStatistics;
    };

}


#endif //SPTAGLIB_POSTINGSTATISTICS_H
