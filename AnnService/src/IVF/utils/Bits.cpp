#include "inc/IVF/utils/Bits.h"


namespace IVF {
    bool Bits::get(DocId docId) const {
        //TODO TESTC
        return true;

        if (docId >= size) {
            //ERROR
        }
        return value[docId];
    }

    int Bits::getSize() const {
        return size;
    }

    Bits::Bits(int size) : size(size) {
        if (size > 0) {
            //TODO max size limit
            value = new bool[size];
            std::memset(value, -1, size);
        }
        //TODO ERROR
    }

    void Bits::setDel(DocId docId) {
        if (docId >= size) {
            //ERROR
        }
        value[docId] = false;
    }

    Bits::~Bits() {
        delete[] value;
    }
}
