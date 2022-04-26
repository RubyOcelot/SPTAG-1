#ifndef SPTAGLIB_TERMSETDATAHOLDER_H
#define SPTAGLIB_TERMSETDATAHOLDER_H

#include <memory>
#include <vector>
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"

namespace IVF {

    class TermSetDataHolder {
    public:
        class TermData {
        public:
            explicit TermData(std::string str="",std::unique_ptr<KeywordStatistic> kwstat= nullptr,std::string posting_data="");
            std::string str;
            std::unique_ptr<KeywordStatistic> kwstat;
            std::string posting_data;
        };
        std::unique_ptr<CollectionStatistic> cstat;
        int termNum;
        std::vector<TermData> termDataVec;
        ~TermSetDataHolder();
    };


}

#endif //SPTAGLIB_TERMSETDATAHOLDER_H
