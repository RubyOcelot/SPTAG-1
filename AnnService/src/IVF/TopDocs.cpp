#include <algorithm>
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

    void TopDocs::print_id_sort() {
        std::vector<DocId> id_list;
        for (auto iter: value) {
            id_list.push_back(iter.docId);
        }
        std::sort(id_list.begin(), id_list.end());
        for (auto iter: id_list) {
            std::cout << iter << " ";
        }
        std::cout << std::endl;
    }

}