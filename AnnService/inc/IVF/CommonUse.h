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

#include "inc/Core/Common.h"

namespace IVF {

    //TODO replace macro
#define MAX_DOCID INT64_MAX

    typedef int64_t DocId;
    typedef int32_t HeadIDType;

    enum LogicOperator {
        AND, OR
    };

    class IndexConfig;
    class IndexSearcher;

    class ScoreScheme;
    typedef ScoreScheme PostingItem;

    enum SourceDataType{
        txt
    };

}
#endif //IVF_FORWARDDEFINE_H
