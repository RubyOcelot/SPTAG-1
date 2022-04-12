#ifndef SPTAGLIB_TERMINDEX_H
#define SPTAGLIB_TERMINDEX_H

#include "inc/IVF/interfaces/KeywordIndex.h"

namespace IVF {

    class TermIndex : public KeywordIndex {
        bool updateAllFinished() override;
    };

}

#endif //SPTAGLIB_TERMINDEX_H
