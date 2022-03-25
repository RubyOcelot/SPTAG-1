#ifndef SPTAGLIB_VECTORSCORESCHEMEFACTORY_H
#define SPTAGLIB_VECTORSCORESCHEMEFACTORY_H

#include <memory>
#include "../interfaces/VectorScoreScheme.h"

namespace  IVF {

    template<typename T>
    class VectorScoreSchemeFactoryBase{
    public:
        virtual std::unique_ptr<VectorScoreScheme<T>> produce()=0;
    };

    template<class U,typename T>
    class VectorScoreSchemeFactory: public VectorScoreSchemeFactoryBase<T>{
    public:
        std::unique_ptr<U> sample;
        VectorScoreSchemeFactory(std::unique_ptr<U> d): sample(std::move(d)){};

        std::unique_ptr<VectorScoreScheme<T>> produce() override{
            return std::make_unique<U>(*sample);//need proper copy constructor
        }

        void print_sample() {
            std::cout << sample.get() << " " <<std::endl;
        }
    };
    template class VectorScoreSchemeFactoryBase<float>;
    template class VectorScoreSchemeFactoryBase<int8_t>;

}

#endif //SPTAGLIB_VECTORSCORESCHEMEFACTORY_H
