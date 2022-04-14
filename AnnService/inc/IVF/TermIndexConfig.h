#ifndef SPTAGLIB_TERMINDEXCONFIG_H
#define SPTAGLIB_TERMINDEXCONFIG_H

#include <string>
#include "inc/IVF/interfaces/IndexConfig.h"

namespace IVF {
    class TermIndexConfig : public IndexConfig {
    public:
        void init(const std::string &dir, IndexSearcher &searcher) override;

        void close() override;
    };
}

#endif //SPTAGLIB_TERMINDEXCONFIG_H
