#include "inc/IVF/utils/VectorDictReturnValue.h"


namespace IVF {
    VectorDictReturnValue::VectorDictReturnValue(std::string key, std::string kwStat) : key(std::move(key)),
                                                                                        kwStat(std::move(kwStat)) {

    }
}