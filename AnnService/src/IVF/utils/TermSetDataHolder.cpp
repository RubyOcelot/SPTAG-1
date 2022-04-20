#include "inc/IVF/utils/TermSetDataHolder.h"

namespace IVF{
    TermSetDataHolder::~TermSetDataHolder() {
        delete[] termData;
    }
}