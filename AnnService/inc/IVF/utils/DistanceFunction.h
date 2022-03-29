#ifndef IVF_DISTANCEFUNCTION_H
#define IVF_DISTANCEFUNCTION_H

#include <cmath>
#include <vector>

namespace IVF {
    template<class T>
    class DistanceFunction {
    public:
        virtual float calc(const std::vector<T> &, const std::vector<T> &) = 0;
    };

    template<class T>
    class L2DistanceFunction : public DistanceFunction<T> {
    public:
        static inline float square(float a);

        float calc(const std::vector<T> &a, const std::vector<T> &b) override;
    };


    template<class T>
    class CosDistanceFunction : public DistanceFunction<T> {
    public:
        static inline float square(float a);

        float calc(const std::vector<T> &a, const std::vector<T> &b) override;
    };

//TODO
//class InnerProductFunction: public DistanceFunction {
//
//    float calc(const VectorValueType& a,const VectorValueType& b) override;
//};
}

#endif //IVF_DISTANCEFUNCTION_H