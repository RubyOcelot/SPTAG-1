#ifndef SPTAGLIB_SPARSEANDDENSEINDEXCONFIG_H
#define SPTAGLIB_SPARSEANDDENSEINDEXCONFIG_H

#include "inc/IVF/interfaces/IndexConfig.h"
#include "Term.h"
#include "TermIndex.h"
#include "KeyVector.h"
#include "VectorIndexWrapper.h"

namespace IVF {

    class SparseAndDenseIndexConfig : public IndexConfig {
    public:
        class TermAndVector: public Term, public KeyVector{
        public:
            TermAndVector(const Term &term, const KeyVector &kv);
        };
        void init(const std::string &path, IndexSearcher &searcher) override;
        std::unique_ptr<Keyword> getFactory() override;
        void close() override;
        std::unique_ptr<TermAndVector> getTermAndVectorFactory();
    private:
        std::unique_ptr<Term> term;
        std::unique_ptr<KeyVector> kv;

    };
}

#endif //SPTAGLIB_SPARSEANDDENSEINDEXCONFIG_H
