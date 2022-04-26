#include "inc/IVF/utils/TermSetDataHolder.h"

#include <utility>

namespace IVF{
    TermSetDataHolder::~TermSetDataHolder() {
    }

    TermSetDataHolder::TermData::TermData(std::string str, std::unique_ptr<KeywordStatistic> kwstat,
                                          std::string posting_data):str(std::move(str)),kwstat(std::move(kwstat)),posting_data(std::move(posting_data)) {

    }
}