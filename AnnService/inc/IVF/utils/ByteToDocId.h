#ifndef SPTAGLIB_BYTETODOCID_H
#define SPTAGLIB_BYTETODOCID_H

#include "inc/Core/Common.h"
#include "inc/IVF/ForwardDefine.h"

namespace IVF{

    DocId ByteToDocId(const unsigned char* Bytes){
        auto a =  (SPTAG::SizeType*)Bytes;
        return DocId(uint64_t(*a));
    }

}

#endif //SPTAGLIB_BYTETODOCID_H
