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

}

#endif //SPTAGLIB_DATATRANSFORM_H
