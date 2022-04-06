#ifndef IVF_TOPDOCS_H
#define IVF_TOPDOCS_H

#include "ForwardDefine.h"
#include "utils/DocWithScore.h"
#include "utils/LoadTruth.h"

namespace IVF {
    class TopDocs {
    public:
//pair of DocId and score
        TopDocs();

        void print();

        void print_id_sort();

        int compare(TruthWrap& truth);

        void add(const DocWithScore &element);

        void finish();

        std::list<DocWithScore> value;
    };
}

#endif //IVF_TOPDOCS_H
