#include <cstdint>

#include "inc/IVF/utils/DistanceFunction.h"

namespace IVF {

    template<class T>
    DistanceUtilsWrap<T>::DistanceUtilsWrap(SPTAG::DistCalcMethod p_method) {
        distFunc=SPTAG::COMMON::DistanceCalcSelector<T>(p_method);
    }

    template<class T>
    float DistanceUtilsWrap<T>::calc(const void *a, const void *b, SPTAG::DimensionType dim) {
        return (*distFunc)((const T*)a, (const T*)b, dim);
    }

}

#define DefineVectorValueType(Name, Type) \
template class IVF::DistanceUtilsWrap<Type>; \

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType