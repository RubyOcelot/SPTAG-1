#include "inc/IVF/utils/LoadTermData.h"
#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/interfaces/PostingStatistics.h"

namespace IVF {

    class DocData{
    public:
        DocId docId;
        std::vector<std::string> tokens;
    };


    std::vector<std::string> tokenize(std::istream& stream){
        char ch;
        int i=0;
        std::vector<std::string> ret;
        while(true){
            stream.get(ch);
            if(ch=='\n')
                break;
            if(ch==' '){
                while(ch!=' '){
                    stream.get(ch);
                    if(ch=='\n')
                        break;
                }
                if(ch=='\n')
                    break;
                ret.push_back("");
                i++;
            }
            ret.at(i)+=ch;
        };
        return ret;
    }

    void linguisticProcess(std::vector<std::string>& data){

    }

    std::unique_ptr<TermSetDataHolder>
    IVF::LoadTermData::getTermSetData(const std::string &path, const SourceDataType &dataType) {
        int docNum;
        std::vector<DocData> docData;
        std::fstream fs;
        auto mode=std::fstream::in;
        if(dataType==SourceDataType::txt){
            fs.open (path, mode);
            fs>>docNum;
            docData.resize(docNum);
            for(auto i=0;i<docNum;i++){
                fs>>docData.at(i).docId;
                docData.at(i).tokens= tokenize(fs);
                linguisticProcess(docData.at(i).tokens);
            }
            fs.close();
        }
        else{
            mode=mode|std::fstream::binary;
        }

        //TODO test code
        auto simpleData=std::make_unique<TermSetDataHolder>();
        simpleData->termNum=2;
        simpleData->termData=new TermSetDataHolder::TermData[simpleData->termNum];
        simpleData->cstat=std::make_unique<TermTFIDFScoreScheme::DocNum>(3);
        simpleData->termData[0].str="happy";
        simpleData->termData[0].kwstat=std::make_unique<TermTFIDFScoreScheme::DocFreq>(3);
        simpleData->termData[0].posting_data= TermTFIDFScoreScheme::serializePItem(1, 1.0) +
                                              TermTFIDFScoreScheme::serializePItem(2, 0.5) +
                                              TermTFIDFScoreScheme::serializePItem(
                                                                                                 3, 2.0 / 3.0);
        simpleData->termData[1].str="fun";
        simpleData->termData[1].kwstat=std::make_unique<TermTFIDFScoreScheme::DocFreq>(2);
        simpleData->termData[1].posting_data= TermTFIDFScoreScheme::serializePItem(2, 0.5) +
                                              TermTFIDFScoreScheme::serializePItem(3, 1.0 / 3.0);
        return simpleData;
    }
}
