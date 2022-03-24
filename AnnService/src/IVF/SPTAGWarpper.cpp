#include "inc/IVF/SPTAGWarpper.h"

namespace IVF {
    template<class T>
    std::unique_ptr<std::vector<VectorDictReturnValue>>
    SPTAGWarpper<T>::search(std::shared_ptr<std::vector<T>> queryVector) {
        //TODO test code
        auto retVec = std::vector<VectorDictReturnValue>();
        for (int i = 0; i < 3; i++) {
            retVec.push_back(VectorDictReturnValue(std::to_string(i), ""));
        }

        return std::make_unique<std::vector<VectorDictReturnValue>>(retVec);
    }

    template class SPTAGWarpper<float>;
    template class SPTAGWarpper<int8_t>;
}