#ifndef SPTAGLIB_LOADTERMDATA_H
#define SPTAGLIB_LOADTERMDATA_H

#include <memory>
#include "inc/IVF/utils/TermSetDataHolder.h"

namespace IVF {
    class LoadTermData {
    public:
        std::unique_ptr <TermSetDataHolder> getTermSetData(const std::string &path);
    };
}

#endif //SPTAGLIB_LOADTERMDATA_H
