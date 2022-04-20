#ifndef IVF_INDEXCONFIG_H
#define IVF_INDEXCONFIG_H

#include <string>
#include "inc/IVF/CommonUse.h"
#include "inc/IVF/interfaces/Keyword.h"

namespace IVF {

    class IndexConfig {
    public:
        //init ScorerScheme for each index/keyword.
        virtual void init(const std::string &path, IndexSearcher &searcher) = 0;
        virtual std::unique_ptr<Keyword> getFactory()=0;
        virtual void close() = 0;
    };
}

#endif //IVF_INDEXCONFIG_H
