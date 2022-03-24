#include "inc/IVF/utils/DistanceFunction.h"

namespace IVF {
    template<class T>
    float L2DistanceFunction<T>::square(float a) {
        return a * a;
    }

    template<class T>
    float L2DistanceFunction<T>::calc(const std::vector<T> &a, const std::vector<T> &b) {

        if (a.size() != b.size()) {
            //TODO ERROR
        }
        float dist = 0;
        for (int i = 0; i < a.size(); i++) {
            dist += square((float)a[i] - (float)b[i]);
        }
        dist = std::sqrt(dist);
//    std::cout<<dist<<std::endl;

        //TODO not sure
        return std::exp(-1 * dist);
    }

    template<class T>
    float CosDistanceFunction<T>::square(float a) {
        return a * a;
    }

    template<class T>
    float CosDistanceFunction<T>::calc(const std::vector<T> &a, const std::vector<T> &b) {

        if (a.size() != b.size()) {
            //TODO ERROR
        }
        float sum_a2 = 0, sum_b2 = 0, sum_ab = 0;
        float result = 0;
        for (int i = 0; i < a.size(); i++) {
            sum_a2 += square((float)a[i]);
            sum_b2 += square((float)b[i]);
            sum_ab += (float)a[i] * (float)b[i];
        }
        result = (sum_ab) / (std::sqrt(sum_a2 * sum_b2));
        //TODO not sure
        return result + 2;
    }

    template class L2DistanceFunction<float>;
    template class L2DistanceFunction<int8_t>;
}