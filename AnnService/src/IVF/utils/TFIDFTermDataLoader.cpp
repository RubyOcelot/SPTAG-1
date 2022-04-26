#include <istream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <map>

#include "inc/IVF/utils/TFIDFTermDataLoader.h"
#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/interfaces/PostingStatistics.h"
#include "inc/IVF/utils/WhiteSpaceTokenizer.h"
#include "inc/IVF/utils/StopwordsFilter.h"
#include "inc/IVF/utils/EnglishSimpleAnalyzer.h"


namespace IVF {


    TermInDoc::TermInDoc(std::string str,std::string posting_data):str(std::move(str)),posting_data(std::move(posting_data)) {

    }


    std::unique_ptr<TermSetDataHolder>
    IVF::TFIDFTermDataLoader::getTermSetData(const std::string &path, const SourceDataType &dataType) {
        TermTFIDFScoreScheme::TermStatType docNum;
        std::vector<std::unique_ptr<TermInDoc>> termInDocData;

        std::fstream fs;
        auto mode=std::fstream::in;
        if(dataType==SourceDataType::txt){
            const int buffer_size=100000;
            char *buffer=new char[buffer_size];
            fs.open (path, mode);
            fs>>docNum;
            for(auto i=0;i<docNum;i++){
                DocId docId;
                fs>>docId;
                fs.getline(buffer,buffer_size-1);
                auto analyzer=EnglishSimpleAnalyzer();
                auto resultTokenStream=analyzer.getTokenStream(std::string(buffer));
                collectData(termInDocData, std::move(resultTokenStream), docId);
            }
            fs.close();
            delete[] buffer;
        }
        else{
            mode=mode|std::fstream::binary;
        }

        //TODO multi-thread sort
        std::sort(termInDocData.begin()+1,termInDocData.end());

        auto retData= concat(termInDocData);
        retData->cstat=std::make_unique<TermTFIDFScoreScheme::DocNum>(docNum);

        return retData;
    }

    void TFIDFTermDataLoader::collectData(std::vector<std::unique_ptr<TermInDoc>> &termInDocData,
                                     std::unique_ptr<TokenStream> tokenStream, DocId docId) {
        std::map<std::string,TermTFIDFScoreScheme::TermStatType> termFreqData;
        while (tokenStream->fetchNextToken()){
            if(termFreqData.find(tokenStream->getToken())==termFreqData.end()){
                termFreqData.insert(std::pair<std::string,TermTFIDFScoreScheme::TermStatType>(tokenStream->getToken(),1));
            }
            else{
                termFreqData.at(tokenStream->getToken())++;
            }
        }
        tokenStream->end();
        tokenStream->close();

        for ( const auto &iter : termFreqData ) {
            termInDocData.push_back(std::make_unique<TermInDoc>(iter.first,TermTFIDFScoreScheme::TermFreq(docId,iter.second).getContent()));
        }
    }

    std::unique_ptr<TermSetDataHolder>
    TFIDFTermDataLoader::concat(std::vector<std::unique_ptr<TermInDoc>> &termInDocData) {
        TermTFIDFScoreScheme::TermStatType curTermNum=0;
        auto retData=std::make_unique<TermSetDataHolder>();
        termInDocData.push_back(std::make_unique<TermInDoc>());
        TermTFIDFScoreScheme::TermStatType docFreq=0;
        std::string posting_data;
        for(auto i=0;i<termInDocData.size()-1;i++){
            docFreq++;
            posting_data+=termInDocData[i]->posting_data;
            if(termInDocData[i]->str!=termInDocData[i+1]->str){
                retData->termDataVec.emplace_back(termInDocData[i]->str,std::make_unique<TermTFIDFScoreScheme::DocFreq>(docFreq),posting_data);
                curTermNum++;
                docFreq=0;
                posting_data="";
            }
        }
        retData->termNum=curTermNum;
        return retData;
    }

}
