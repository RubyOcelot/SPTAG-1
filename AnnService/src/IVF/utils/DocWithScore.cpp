#include "inc/IVF/utils/DocWithScore.h"

namespace IVF {
    void DocWithScore::print() const {
        std::cout << "doc_id: " << docId << " score: " << score << std::endl;
    }

    DocWithScore::DocWithScore() : docId(-1), score(-1) {

    }

    bool DocWithScore::operator==(const DocWithScore &a) const {
        return (docId == a.docId) && (score == a.score);
    }

    DocWithScore::DocWithScore(DocId docId, float score) : docId(docId), score(score) {

    }
}