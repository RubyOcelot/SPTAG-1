#include "inc/IVF/DefaultVectorScoreScheme.h"

#include <utility>
//#include "postingItem.pb.h"
#include <inc/Core/Common.h>

namespace IVF {
    template class DefaultVectorScoreScheme<float>;
    template class DefaultVectorScoreScheme<int8_t>;

    template<class T>
    VectorScoreScheme *DefaultVectorScoreScheme<T>::clone() {
        return new DefaultVectorScoreScheme( distFunc, queryVector, docId, docVector, vecLen);
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

    DocId ByteToDocId(const unsigned char* Bytes){
        auto a =  (SPTAG::SizeType*)Bytes;
        return DocId(uint64_t(*a));
    }

    template<class T>
    bool DefaultVectorScoreScheme<T>::postingStatisticsLoader(std::istream *rawStream) {
//         not using protobuf currently
//        //TODO TESTC
//        pb::postingItem p1;
//        //TODO fix consume entire input
//        p1.ParseFromIstream(rawStream);
//        //TODO check vector len
//        if (p1.has_docid() && p1.vecval_size() > 0) {
//            docId = p1.docid();
//
//            docVector = std::vector<T>();
//            for (int i = 0; i < p1.vecval_size(); i++) {
//                docVector.push_back(p1.vecval(i));
//            }
//            return true;
//        } else {
//            return false;
//        }



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



}


