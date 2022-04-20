#ifndef SPTAGLIB_KEYWORDSTATISTIC_H
#define SPTAGLIB_KEYWORDSTATISTIC_H

#include <string>
#include <memory>

namespace IVF {

    class KeywordStatistic {
    public:
        virtual void reset() = 0;

        virtual void set(const std::string &) = 0;

        virtual void modForAdd(const std::string &) = 0;

        virtual void modForDel(const std::string &) = 0;

        virtual std::string getContent() = 0;

        virtual std::unique_ptr<KeywordStatistic> clone() = 0;

        virtual std::unique_ptr<KeywordStatistic> getNew() = 0;
    };

}
#endif //SPTAGLIB_KEYWORDSTATISTIC_H
