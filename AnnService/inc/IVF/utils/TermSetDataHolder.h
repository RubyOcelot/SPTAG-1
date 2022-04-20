#ifndef SPTAGLIB_TERMSETDATAHOLDER_H
#define SPTAGLIB_TERMSETDATAHOLDER_H

#include <memory>
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/Term.h"

namespace IVF {

    class TermSetDataHolder {
    public:
        class TermData {
        public:
            Term term;
            std::unique_ptr<KeywordStatistic> kwstat;
            std::string posting_data;
        };
        std::unique_ptr<CollectionStatistic> cstat;
        int termNum;
        TermData* termData;
        std::unique_ptr<KeywordStatistic>emptyStat;
        ~TermSetDataHolder();
    };


}

#endif //SPTAGLIB_TERMSETDATAHOLDER_H
