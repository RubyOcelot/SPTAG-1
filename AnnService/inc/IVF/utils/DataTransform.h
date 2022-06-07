#ifndef SPTAGLIB_DATATRANSFORM_H
#define SPTAGLIB_DATATRANSFORM_H

#include "inc/Core/Common.h"
#include "inc/IVF/CommonUse.h"

namespace IVF{
    DocId ByteToDocId(const unsigned char* Bytes);

    template<class T>
    static std::string DataToString(const T& data){
        std::string s;
        for(auto i=0;i<sizeof(data);i++){
            s+=*((char*)(&data)+i);
        }
        return s;
    }

    static std::string BufferToString(char* buffer,int size){
        std::string s;
        for(auto i=0;i<size;i++){
            s+=buffer[i];
        }
        return s;
    }


    class inputData{
    public:
        inputData(DocId docId, char* buffer);
        DocId docId;
        std::string docStr;
    };


}

#endif //SPTAGLIB_DATATRANSFORM_H
