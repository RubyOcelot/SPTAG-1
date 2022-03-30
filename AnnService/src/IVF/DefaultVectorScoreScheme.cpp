#include "inc/IVF/DefaultVectorScoreScheme.h"

#include <utility>
//#include "postingItem.pb.h"


namespace IVF {
    template class DefaultVectorScoreScheme<float>;
    template class DefaultVectorScoreScheme<int8_t>;

    template<class T>
    VectorScoreScheme<T> *DefaultVectorScoreScheme<T>::clone() {
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
    DefaultVectorScoreScheme<T>::DefaultVectorScoreScheme(std::shared_ptr<DistanceFunction<T>> distFunc, std::shared_ptr<std::vector<T>> queryVector, DocId docId, std::shared_ptr<std::vector<T>> docVector,  int vecLen)
            : docId(docId), docVector(docVector), queryVector(std::move(queryVector)), distFunc(std::move(distFunc)), vecLen(vecLen) {

    }


    template<class T>
    float DefaultVectorScoreScheme<T>::score() {
        //calc vector distance
        //TODO not sure about score and distance
        return (-1)*distFunc->calc((*docVector), *(queryVector));
    }

    DocId ByteToDocId(const unsigned char* Bytes){
        auto a =  (uint32_t)(((unsigned char)(Bytes[0]) << 24 |
                    (unsigned char)(Bytes[1]) << 16 |
                    (unsigned char)(Bytes[2]) << 8 |
                    (unsigned char)(Bytes[3])));

        return DocId(uint64_t(a));
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
        docVector=std::make_shared<std::vector<T>>();
        if(*rawStream){
            auto* buffer=new unsigned char[vecLen*sizeof(T)];
            //TODO vector dim size
            //TODO async
            rawStream->read((char*)(buffer), 4);
            if(rawStream->gcount()!=4)return false;
            docId= ByteToDocId(buffer);
            for(int i=0;i<vecLen;i++){
                if(!*rawStream) return false;
                T* value;
                rawStream->read((char*)(buffer), sizeof(T));
                if(rawStream->gcount()!=sizeof(T))return false;
                value=(T*)buffer;
                docVector->push_back(*value);
            }
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
    void DefaultVectorScoreScheme<T>::setQueryVector(std::shared_ptr<std::vector<T>> qVector) {
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


