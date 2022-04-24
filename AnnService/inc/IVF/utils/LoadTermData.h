#ifndef SPTAGLIB_LOADTERMDATA_H
#define SPTAGLIB_LOADTERMDATA_H

#include <memory>
#include "inc/IVF/utils/TermSetDataHolder.h"
#include "inc/IVF/CommonUse.h"

namespace IVF {
    class LoadTermData {
    public:
        std::unique_ptr<TermSetDataHolder> getTermSetData(const std::string &path, const SourceDataType &dataType);
    };
}

#endif //SPTAGLIB_LOADTERMDATA_H
