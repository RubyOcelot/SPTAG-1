#ifndef SPTAGLIB_DEFAULTTERMINDEXCONFIG_H
#define SPTAGLIB_DEFAULTTERMINDEXCONFIG_H

#include <string>
#include "inc/IVF/interfaces/IndexConfig.h"
#include "Term.h"
#include "TermIndex.h"

namespace IVF {
    class DefaultTermIndexConfig : public IndexConfig {
    public:
        void init(const std::string &path, IndexSearcher &searcher) override;
        std::unique_ptr<Keyword> getFactory() override;
        void close() override;
        std::unique_ptr<Term> getTermFactory();
    private:
        std::shared_ptr<TermIndex> termIndex;
    };
}

#endif //SPTAGLIB_DEFAULTTERMINDEXCONFIG_H
