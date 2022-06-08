#ifndef IVF_TOPDOCS_H
#define IVF_TOPDOCS_H

#include "inc/IVF/CommonUse.h"
#include "DocWithScore.h"
#include "LoadTruth.h"

namespace IVF {
    class TopDocs {
    public:
//pair of DocId and score
        TopDocs();

        void print();

        void print_id_sort();

        void add(const DocWithScore &element);

        int getDocNum();

        void reverse();

        DocId getIdAt(int index);

        std::shared_ptr<std::vector<DocWithScore>> value;
    };
}

#endif //IVF_TOPDOCS_H
