

#ifndef IVF_SPTAGWARPPER_H
#define IVF_SPTAGWARPPER_H

#include <vector>
#include <memory>
#include "utils/VectorDictReturnValue.h"

namespace IVF {
    template<class T>
    class SPTAGWarpper {
    public:
        std::unique_ptr<std::vector<VectorDictReturnValue>> search(std::shared_ptr<std::vector<T>> queryVector);

    };

}

#endif //IVF_SPTAGWARPPER_H
