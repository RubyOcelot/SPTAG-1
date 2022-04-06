#ifndef IVF_DEFAULTVECTORINDEXCONFIG_H
#define IVF_DEFAULTVECTORINDEXCONFIG_H

#include "interfaces/IndexConfig.h"


namespace IVF {
    class DefaultVectorIndexConfig : public IndexConfig {
    public:
        void init(const std::string &dir) override;

        void close() override;
    };
}

#endif //IVF_DEFAULTVECTORINDEXCONFIG_H
