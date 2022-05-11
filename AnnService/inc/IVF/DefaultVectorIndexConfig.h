#ifndef IVF_DEFAULTVECTORINDEXCONFIG_H
#define IVF_DEFAULTVECTORINDEXCONFIG_H

#include "interfaces/IndexConfig.h"
#include "inc/IVF/KeyVector.h"


namespace IVF {
    class DefaultVectorIndexConfig : public IndexConfig {
    public:
        void init(const std::string &path, IndexSearcher &searcher) override;
        std::unique_ptr<Keyword> getFactory() override;
        void close() override;
        std::unique_ptr<KeyVector> getVectorFactory();
        SPTAG::DistCalcMethod vectorDistCalcMethod;
    private:
        std::shared_ptr<VectorIndexWrapper> vectorIndexWrapper;
    };
}

#endif //IVF_DEFAULTVECTORINDEXCONFIG_H
