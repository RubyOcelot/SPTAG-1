#ifndef IVF_DISTANCEFUNCTION_H
#define IVF_DISTANCEFUNCTION_H

#include <cmath>
#include <vector>

#include "inc/Core/Common.h"
#include "inc/Core/Common/DistanceUtils.h"

namespace IVF {
    template <typename T>
    using DistanceCalcReturn = float(*)(const T*, const T*, SPTAG::DimensionType);

    class DistanceFunction {
    public:
        virtual float calc(const void*,const void*,SPTAG::DimensionType) = 0;
    };

    template<class T>
    class DistanceUtilsWrap : public DistanceFunction {
    public:
        DistanceCalcReturn<T> distFunc;
        explicit DistanceUtilsWrap(SPTAG::DistCalcMethod p_method);
        float calc(const void* a, const void* b,SPTAG::DimensionType) override;

    };

//TODO InnerProductFunction

}

#endif //IVF_DISTANCEFUNCTION_H