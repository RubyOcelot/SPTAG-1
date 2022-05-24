#include "inc/IVF/utils/DataTransform.h"

namespace IVF{

    DocId ByteToDocId(const unsigned char* Bytes){
        auto a =  (SPTAG::SizeType*)Bytes;
        return DocId(uint64_t(*a));
    }

//    template<class T>
//    static std::string DataToString(const T& data){
//        std::string s;
//        for(auto i=0;i<sizeof(data);i++){
//            s+=*((char*)(&data)+i);
//        }
//        return s;
//    }
    inputData::inputData(DocId docId, char *buffer): docId(docId),docStr(std::string(buffer)) {

    }
}