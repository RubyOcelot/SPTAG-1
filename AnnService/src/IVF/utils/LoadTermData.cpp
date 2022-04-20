#include "inc/IVF/utils/LoadTermData.h"
#include "inc/IVF/TermTfidfScoreScheme.h"

namespace IVF {

    std::unique_ptr<TermSetDataHolder> IVF::LoadTermData::getTermSetData(const std::string &path) {
        //TODO test code
        auto data=std::make_unique<TermSetDataHolder>();
        data->termNum=2;
        data->termData=new TermSetDataHolder::TermData[data->termNum];
        data->cstat=std::make_unique<TermTFIDFScoreScheme::DocNum>(3);
        data->termData[0].str="happy";
        data->termData[0].kwstat=std::make_unique<TermTFIDFScoreScheme::DocFreq>(3);
        data->termData[0].posting_data=TermTFIDFScoreScheme::serializePostingStat(1,1.0)+TermTFIDFScoreScheme::serializePostingStat(2,0.5)+TermTFIDFScoreScheme::serializePostingStat(3,2.0/3.0);
        data->termData[1].str="fun";
        data->termData[1].kwstat=std::make_unique<TermTFIDFScoreScheme::DocFreq>(2);
        data->termData[1].posting_data=TermTFIDFScoreScheme::serializePostingStat(2,0.5)+TermTFIDFScoreScheme::serializePostingStat(3,1.0/3.0);
        return data;
    }
}
