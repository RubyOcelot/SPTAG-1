#include "inc/IVF/utils/DataTransform.h"

namespace IVF{

    DocId ByteToDocId(const unsigned char* Bytes){
        auto a =  (SPTAG::SizeType*)Bytes;
        return DocId(uint64_t(*a));
    }
}