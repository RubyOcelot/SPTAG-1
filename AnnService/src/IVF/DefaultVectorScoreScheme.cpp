#include <utility>
#include "inc/Core/Common.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include "inc/IVF/utils/DataTransform.h"

namespace IVF {

    template<class T>
    VectorScoreScheme *DefaultVectorScoreScheme<T>::clone() {
        return new DefaultVectorScoreScheme( distFunc, queryVector, docId, docVector, vecLen);
    }

    template<class T>
    std::unique_ptr<ScoreScheme> DefaultVectorScoreScheme<T>::smart_clone() {
        return std::make_unique<DefaultVectorScoreScheme>( distFunc, queryVector, docId, docVector, vecLen);
    }

    template<class T>
    DefaultVectorScoreScheme<T>::DefaultVectorScoreScheme(const DefaultVectorScoreScheme& d):
            distFunc(d.distFunc),
            queryVector(d.queryVector),
            docId(d.docId),
            docVector(d.docVector),
            vecLen(d.vecLen){}

    template<class T>
    DocId DefaultVectorScoreScheme<T>::getDocId() {
        return docId;
    }

    template<class T>
    DefaultVectorScoreScheme<T>::DefaultVectorScoreScheme(std::shared_ptr<DistanceFunction> distFunc, void* queryVector, DocId docId, void* docVector,  int vecLen)
            : docId(docId), docVector(docVector), queryVector(queryVector), distFunc(std::move(distFunc)), vecLen(vecLen) {

    }

    template<class T>
    float DefaultVectorScoreScheme<T>::score() {
        //calc vector distance
        //TODO not sure about score and distance
        return (-1)*distFunc->calc(docVector, queryVector, vecLen);
    }

    template<class T>
    bool DefaultVectorScoreScheme<T>::postingStatisticsLoader(std::istream *rawStream) {

        docVector=new T[vecLen];
        if(*rawStream){
            auto* buffer=new unsigned char[sizeof(SPTAG::SizeType)];
            //TODO vector dim size
            //TODO async
            rawStream->read((char*)(buffer), 4);
            if(rawStream->gcount()!=4){
                return false;
            }

            docId = ByteToDocId(buffer);

            rawStream->read((char*)(docVector), vecLen*sizeof(T));
            if(rawStream->gcount()!=vecLen*sizeof(T))
                return false;

            delete[] buffer;
            return true;
        }
        else{
            return false;
        }
    }


    template<class T>
    void DefaultVectorScoreScheme<T>::keywordStatisticsLoader(std::string) {
        //TODO impl
    }

    template<class T>
    void DefaultVectorScoreScheme<T>::collectionStatisticsLoader(std::string str) {
        vecLen=std::stoi( str );
    }



    template<class T>
    void DefaultVectorScoreScheme<T>::setQueryVector(void* qVector) {
        this->queryVector = qVector;

    }

    template<class T>
    DefaultVectorScoreScheme<T>::~DefaultVectorScoreScheme() {
        //TODO impl
    }


    template<class T>
    void DefaultVectorScoreScheme<T>::setVecLen(int len) {
        vecLen=len;
    }

    template<class T>
    int DefaultVectorScoreScheme<T>::getVecLen() {
        return vecLen;
    }

    template<class T>
    std::unique_ptr<KeywordStatistic> DefaultVectorScoreScheme<T>::getEmptyKeywordStatistic() {
        return std::unique_ptr<KeywordStatistic>();
    }

    template<class T>
    SPTAG::DistCalcMethod DefaultVectorScoreScheme<T>::getDefaultDistCalcFunc() {
        return defaultDistCalcFunc;
    }

    template<class T>
    std::unique_ptr<CollectionStatistic> DefaultVectorScoreScheme<T>::getEmptyCollectionStatistic() {
        return nullptr;
    }

}

#define DefineVectorValueType(Name, Type) \
template class IVF::DefaultVectorScoreScheme<Type>; \

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType

