#ifndef IVF_FORWARDDEFINE_H
#define IVF_FORWARDDEFINE_H

#include <vector>
#include <cstdint>
#include <list>
#include <string>
#include <bitset>
#include <utility>
#include <iostream>
#include <cmath>
#include <cstring>
#include <memory>

namespace IVF {

    //TODO replace macro
#define MAX_DOCID INT64_MAX

    typedef int64_t DocId;

    class DocWithScore;

    class DocPriorityQueue;

    class Bits;

    class TopDocs;

    class SubScorerSet;

    enum LogicOperator {
        AND, OR
    };

    class Scorer;

    class SimpleScorer;

    class BooleanScorer;

    class Keyword;

    class KeyVector;

    class Query;

    class KeywordQuery;

    class BooleanQuery;

//TODO useless
//class PostingItem;
//class vectorPostingStatistics;
//class KeywordStatistics;
//class CollectionStatistics;
    class ScoreScheme;


    typedef ScoreScheme PostingItem;

    class PostingStatistics;

//
    class Collector;

    class DocVectorItem;

    class IndexConfig;
}
#endif //IVF_FORWARDDEFINE_H
