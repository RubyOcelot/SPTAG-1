#include <istream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <map>
#include <thread>

#include "inc/IVF/utils/TFIDFTermDataLoader.h"
#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/interfaces/CollectionStatistic.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/interfaces/PostingStatistics.h"
#include "inc/IVF/utils/WhiteSpaceTokenizer.h"
#include "inc/IVF/utils/StopwordsFilter.h"
#include "inc/IVF/utils/EnglishSimpleAnalyzer.h"


namespace IVF {


    TermInDoc::TermInDoc(std::string str,std::string posting_data,DocId docId):str(std::move(str)),posting_data(std::move(posting_data)),docId(docId) {

    }

    class inputData{
    public:
        inputData(DocId docId, char* buffer);
        DocId docId;
        std::string docStr;
    };

    inputData::inputData(DocId docId, char *buffer): docId(docId),docStr(std::string(buffer)) {

    }


    std::unique_ptr<TermSetDataHolder>
    IVF::TFIDFTermDataLoader::getTermSetData(const std::string &path, const SourceDataType &dataType,
                                             const int threadNum) {
        TermTFIDFScoreScheme::TermStatType docNum;
        std::vector<std::shared_ptr<TermInDoc>> termInDocData;

        std::shared_ptr<std::vector<inputData>> inputDataVec=std::make_shared<std::vector<inputData>>();

        std::fstream fs;
        auto mode=std::fstream::in;
        if(dataType==SourceDataType::txt){
            const int buffer_size=100000;
            char *buffer=new char[buffer_size];
            fs.open (path, mode);

            SPTAG::LOG(SPTAG::Helper::LogLevel::LL_Info, "Inverted Index Build: start reading data, datatype=txt\n");
            fs>>docNum;
            for(auto i=0;i<docNum;i++){
                DocId docId;
                fs>>docId;
                char c;
                fs.get(c);
                if(fs.gcount()>0 && c!='\n'){
                    fs.putback(c);
                }
                fs.getline(buffer,buffer_size-1);
//                auto x=fs.gcount();
                inputDataVec->push_back(inputData(docId,buffer));
                fs.get(c);
                if(fs.gcount()>0 && c!='\n'){
                    fs.putback(c);
                }
            }
            fs.close();
            delete[] buffer;
        }
        else if(dataType==SourceDataType::tsv){
            const int buffer_size=100000;
            char *buffer=new char[buffer_size];
            fs.open (path, mode);

            SPTAG::LOG(SPTAG::Helper::LogLevel::LL_Info, "Inverted Index Build: start reading data, datatype=tsv\n");
            docNum=0;
            while(!fs.eof()){
                DocId docId;
                fs>>docId;
                fs.getline(buffer,buffer_size-1);
//                auto x=fs.gcount();
                inputDataVec->push_back(inputData(docId,buffer));
                docNum++;
                char c;
                fs.get(c);
                if(fs.gcount()>0 && c!='\n'){
                    fs.putback(c);
                }
            }
            fs.close();
            delete[] buffer;
        }
        else if(dataType==SourceDataType::bin){
            mode=mode|std::fstream::binary;
        }

        std::cout<<"docNum: "<<docNum<<std::endl;

        SPTAG::LOG(SPTAG::Helper::LogLevel::LL_Info, "Inverted Index Build: start tokenizing\n");

        auto analyzer=EnglishSimpleAnalyzer();

        std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<TermInDoc>>>>> termInDocDataCollect(new std::vector<std::shared_ptr<std::vector<std::shared_ptr<TermInDoc>>>>);
        termInDocDataCollect->resize(docNum);

        std::vector<std::thread> task_threads;
        std::atomic_size_t task_sent(0);
        std::atomic_size_t itemNum(0);

        auto func = [&]() {
            size_t index = 0;
            while (true) {
                index = task_sent.fetch_add(1);
                if (index < docNum) {
                    auto i=index;
                    std::shared_ptr<std::vector<std::shared_ptr<TermInDoc>>> termInDocDataLocal(new std::vector<std::shared_ptr<TermInDoc>>);
                    auto resultTokenStream=analyzer.getTokenStream(inputDataVec->at(i).docStr);
                    collectData(termInDocDataLocal, std::move(resultTokenStream), inputDataVec->at(i).docId);
                    termInDocDataCollect->at(i)=termInDocDataLocal;
                    itemNum+=termInDocDataLocal->size();
                } else {
                    return;
                }
            }
        };
        for (int j = 0; j < threadNum; j++) { task_threads.emplace_back(func); }
        for (auto &thread: task_threads) { thread.join(); }


//        for(auto i=0;i<inputDataVec->size();i++){
//            std::shared_ptr<std::vector<std::shared_ptr<TermInDoc>>> termInDocDataLocal(new std::vector<std::shared_ptr<TermInDoc>>);
//            auto resultTokenStream=analyzer.getTokenStream(inputDataVec->at(i).docStr);
//            collectData(termInDocDataLocal, std::move(resultTokenStream), inputDataVec->at(i).docId);
//            termInDocDataCollect->at(i)=termInDocDataLocal;
//            itemNum+=termInDocDataLocal->size();
//        }

        termInDocData.reserve(itemNum);
        for(auto i=0;i<termInDocDataCollect->size();i++) {
            for(const auto& iter:*termInDocDataCollect->at(i)){
                termInDocData.push_back(iter);
            }
        }

        SPTAG::LOG(SPTAG::Helper::LogLevel::LL_Info, "Inverted Index Build: start sorting\n");

        //TODO multi-thread sort
         std::sort(termInDocData.begin(),termInDocData.end(), [] (const std::shared_ptr<TermInDoc>& a, const std::shared_ptr<TermInDoc>& b) {
             int condition=a->str.compare(b->str);
             return (condition==0)?(a->docId<b->docId):(condition<0); });

//        for(auto iter:termInDocData){
//            std::cout<<iter->str<<" ";
//        }
//        std::cout<<std::endl;

        SPTAG::LOG(SPTAG::Helper::LogLevel::LL_Info, "Inverted Index Build: start concating\n");
        auto retData= concat(termInDocData);
        retData->cstat=std::make_unique<TermTFIDFScoreScheme::DocNum>(docNum);

        return retData;
    }

    void TFIDFTermDataLoader::collectData(const std::shared_ptr<std::vector<std::shared_ptr<TermInDoc>>>& termInDocData,
                                     std::unique_ptr<TokenStream> tokenStream, DocId docId) {
        std::map<std::string,TermTFIDFScoreScheme::TermStatType> termFreqData;
        int tokenCount=0;
        while (tokenStream->fetchNextToken()){
            tokenCount++;
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
            termInDocData->push_back(std::make_unique<TermInDoc>(iter.first,TermTFIDFScoreScheme::TermFreq(docId,(float)iter.second/(float)tokenCount).getContent(),docId));
        }
    }

    std::unique_ptr<TermSetDataHolder>
    TFIDFTermDataLoader::concat(std::vector<std::shared_ptr<TermInDoc>> &termInDocData) {
        int termNum=0;
        auto retData=std::make_unique<TermSetDataHolder>();
        termInDocData.push_back(std::make_unique<TermInDoc>());
        TermTFIDFScoreScheme::TermStatType docFreq=0;
        std::string posting_data;
        for(auto i=0;i<termInDocData.size()-1;i++){
            docFreq++;
            posting_data+=termInDocData[i]->posting_data;
            if(termInDocData[i]->str!=termInDocData[i+1]->str){
                retData->termDataVec.emplace_back(termInDocData[i]->str,std::make_unique<TermTFIDFScoreScheme::DocFreq>(docFreq),posting_data);
                termNum++;
                docFreq=0;
                posting_data="";
            }
        }
        retData->termNum=termNum;
        return retData;
    }

}
