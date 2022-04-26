#ifndef SPTAGLIB_TERMDATALOADER_H
#define SPTAGLIB_TERMDATALOADER_H

#include "inc/IVF/utils/TermSetDataHolder.h"
#include "inc/IVF/CommonUse.h"

namespace IVF {
    class TermDataLoader {
    public:
        virtual std::unique_ptr<TermSetDataHolder> getTermSetData(const std::string &path, const SourceDataType &dataType)=0;
    };
}

#endif //SPTAGLIB_TERMDATALOADER_H
