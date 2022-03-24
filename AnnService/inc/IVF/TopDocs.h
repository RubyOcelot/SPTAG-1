#ifndef IVF_TOPDOCS_H
#define IVF_TOPDOCS_H

#include "ForwardDefine.h"
#include "utils/DocWithScore.h"

namespace IVF {
    class TopDocs {
    public:
//pair of DocId and score
        TopDocs();

        void print();

        void add(const DocWithScore &element);

        void finish();

    private:
        //TODO replace list
        std::list<DocWithScore> value;
    };
}

#endif //IVF_TOPDOCS_H
