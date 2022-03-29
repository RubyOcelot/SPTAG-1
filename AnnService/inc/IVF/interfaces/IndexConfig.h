#ifndef IVF_INDEXCONFIG_H
#define IVF_INDEXCONFIG_H

#include <string>

namespace IVF {

    class IndexConfig {
    public:
        //init ScorerScheme for each index/keyword.
        virtual void init(const std::string &dir) = 0;

        virtual void close() = 0;
    };
}

#endif //IVF_INDEXCONFIG_H
