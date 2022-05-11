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

        int compare(TruthWrap& truth);

        void add(const DocWithScore &element);

        int getDocNum();

        void reverse();

        std::shared_ptr<std::vector<DocWithScore>> value;
    };
}

#endif //IVF_TOPDOCS_H
