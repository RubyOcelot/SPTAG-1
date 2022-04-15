#include "inc/IVF/TermIndexConfig.h"

namespace IVF {

    std::unique_ptr<Keyword> IVF::TermIndexConfig::getFactory() {
        return getTermFactory();
    }

    std::unique_ptr<Term> IVF::TermIndexConfig::getTermFactory() {
        return std::make_unique<Term>("",termIndex);
    }

    void TermIndexConfig::init(const std::string &dir, IndexSearcher &searcher) {
        termIndex=std::make_shared<TermIndex>();
        searcher.indexCollection.push_back(termIndex);
    }
}
