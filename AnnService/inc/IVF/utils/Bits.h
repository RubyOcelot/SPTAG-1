#ifndef IVF_BITS_H
#define IVF_BITS_H

#include "../ForwardDefine.h"

namespace IVF {
    class Bits {
    public:
        Bits(int size);

        bool get(DocId docId) const;

        void setDel(DocId docId);

        int getSize() const;

        ~Bits();

    private:
        int size;
        bool *value;
    };
}

#endif //IVF_BITS_H
