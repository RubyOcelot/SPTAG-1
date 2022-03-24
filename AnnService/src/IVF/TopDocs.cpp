#include "inc/IVF/TopDocs.h"

namespace IVF {
    void TopDocs::print() {
        for (auto iter: value) {
            iter.print();
        }
    }

    TopDocs::TopDocs() {
        value = std::list<DocWithScore>();
    }

    void TopDocs::add(const DocWithScore &element) {
        value.push_front(element);
    }

    void TopDocs::finish() {
//may need reverse if not list
    }
}