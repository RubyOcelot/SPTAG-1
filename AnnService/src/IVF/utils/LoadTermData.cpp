#include "inc/IVF/utils/LoadTermData.h"

namespace IVF {

    std::unique_ptr<TermSetDataHolder> IVF::LoadTermData::getTermSetData(const std::string &path) {
        return std::unique_ptr<TermSetDataHolder>();
    }
}
