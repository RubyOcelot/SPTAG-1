#ifndef SPTAGLIB_VECTORSCORESCHEMEFACTORY_H
#define SPTAGLIB_VECTORSCORESCHEMEFACTORY_H

#include <memory>
#include "inc/IVF/interfaces/ScoreScheme.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"

namespace IVF {
    class VectorScoreSchemeFactoryBase {
    public:
        virtual std::unique_ptr<VectorScoreScheme> produce()=0;
        virtual VectorScoreSchemeFactoryBase* clone()=0;
    };

    template<class T>
    class VectorScoreSchemeFactory:public VectorScoreSchemeFactoryBase {
    public:
        std::unique_ptr<T> sample;
        explicit VectorScoreSchemeFactory(std::unique_ptr<T> d):sample(std::move(d)){};
        void init(std::unique_ptr<T> d) {
            sample = std::move(d);
        }
        std::unique_ptr<VectorScoreScheme> produce() override {
            return std::make_unique<T>(*sample);//copy constructor
        }
        VectorScoreSchemeFactoryBase* clone() override{
            return new VectorScoreSchemeFactory(std::make_unique<T>(*sample));
        }
    };

}

#define DefineVectorValueType(Name, Type) \
    template class IVF::ScoreSchemeFactory<IVF::DefaultVectorScoreScheme<Type>>;\

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType


#endif //SPTAGLIB_VECTORSCORESCHEMEFACTORY_H
