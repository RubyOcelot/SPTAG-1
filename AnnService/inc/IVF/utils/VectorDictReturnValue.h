#ifndef IVF_VECTORDICTRETURNVALUE_H
#define IVF_VECTORDICTRETURNVALUE_H

#include <utility>

#include <string>

namespace IVF {
    class VectorDictReturnValue {
    public:
        VectorDictReturnValue(std::string key, std::string kwStat);

        std::string key;
        std::string kwStat;
    };
}

#endif //IVF_VECTORDICTRETURNVALUE_H
