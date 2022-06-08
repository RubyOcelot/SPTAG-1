
#include <cmath>
#include <iostream>
#include <memory>
#include <new>
#include "inc/Test.h"
#include "inc/IVF/api.h"

namespace IVF {

    typedef std::chrono::steady_clock SteadClock;

    double getMsInterval(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
        return (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 1.0) / 1000.0;
    }

    double getSecInterval(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
        return (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() * 1.0) / 1000.0;
    }

    double getMinInterval(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
        return (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() * 1.0) / 60.0;
    }

/// Clock class
    class StopWSPFresh {
    private:
        std::chrono::steady_clock::time_point time_begin;
    public:
        StopWSPFresh() {
            time_begin = std::chrono::steady_clock::now();
        }

        double getElapsedMs() {
            std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
            return getMsInterval(time_begin, time_end);
        }

        double getElapsedSec() {
            std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
            return getSecInterval(time_begin, time_end);
        }

        double getElapsedMin() {
            std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
            return getMinInterval(time_begin, time_end);
        }

        void reset() {
            time_begin = std::chrono::steady_clock::now();
        }
    };



    void test_combine(const std::string &test_dir){
        auto opts = getSPTAGOptions(test_dir.c_str());
        auto indexConfig=std::make_shared<SparseAndDenseIndexConfig>();
        auto searcher = IndexSearcher(test_dir, indexConfig);
        std::shared_ptr<KeyVector> kvFactory=indexConfig->getTermAndVectorFactory();
        std::shared_ptr<Term> termFactory=indexConfig->getTermAndVectorFactory();

        delete opts;
    }




    std::shared_ptr<std::vector<std::vector<std::string>>> getQuery(const std::string &path, const SourceDataType &dataType,
                                                               const int threadNum){

        int queryNum;
        std::shared_ptr<std::vector<inputData>> inputDataVec=std::make_shared<std::vector<inputData>>();

        std::fstream fs;

        auto mode=std::fstream::in;
        if(dataType==SourceDataType::txt){
            const int buffer_size=100000;
            char *buffer=new char[buffer_size];
            fs.open (path, mode);

            fs >> queryNum;
            for(auto i=0; i < queryNum; i++){
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

            queryNum=0;
            while(!fs.eof()){
                DocId docId;
                fs>>docId;
                fs.getline(buffer,buffer_size-1);
//                auto x=fs.gcount();
                inputDataVec->push_back(inputData(docId,buffer));
                queryNum++;
                char c;
                fs.get(c);
                if(fs.gcount()>0 && c!='\n'){
                    fs.putback(c);
                }
            }
            fs.close();
            delete[] buffer;
        }

        std::shared_ptr<std::vector<std::vector<std::string>>> ret(new std::vector<std::vector<std::string>>);

        ret->resize(inputDataVec->size());
        auto analyzer=EnglishSimpleAnalyzer();
        for(auto i=0;i<inputDataVec->size();i++){
            auto resultTokenStream=analyzer.getTokenStream(inputDataVec->at(i).docStr);
            while(resultTokenStream->fetchNextToken()) {
                ret->at(i).push_back(resultTokenStream->getToken());
            }
            resultTokenStream->close();
        }

        return ret;


    }

    void test_term(const std::string &test_dir) {
        StopWSPFresh sw1;

        auto indexConfig=std::make_shared<DefaultTermIndexConfig>();

        auto searcher = IndexSearcher(test_dir, indexConfig);

        double setupCost = sw1.getElapsedSec();
        LOG(Helper::LogLevel::LL_Info,
            "Finish index setup in %.3lf seconds\n",
            setupCost);

        std::shared_ptr<Term> termFactory=indexConfig->getTermFactory();


        Helper::IniReader iniReader;
        iniReader.LoadIniFile(test_dir);
        auto config_map=new std::map<std::string, std::map<std::string, std::string>>;
        (*config_map)["SearchIndex"] = iniReader.GetParameters("SearchIndex");

        if((*config_map)["SearchIndex"]["searchindex"]=="true") {
            int K=10;
            auto sourceFile=(*config_map)["SearchIndex"]["sourcefile"];
            auto sourceFileType=(*config_map)["SearchIndex"]["sourcefiletype"];
            SourceDataType sourceDataType;
            if(sourceFileType=="txt")
                sourceDataType=SourceDataType::txt;
            else if(sourceFileType=="tsv")
                sourceDataType=SourceDataType::tsv;
            else if(sourceFileType=="bin")
                sourceDataType=SourceDataType::bin;
            auto threadNum=std::atoi((*config_map)["SearchIndex"]["numberofthreads"].c_str());

            auto query_data = getQuery(sourceFile,sourceDataType,threadNum);

            int query_num=query_data->size();

            std::shared_ptr<std::vector<TopDocs>> results=std::make_shared<std::vector<TopDocs>>();
            results->resize(query_num);
            StopWSPFresh sw2;

            for(auto i=0;i<query_data->size();i++){

                auto queryList = std::make_unique<std::vector<std::shared_ptr<Query>>>();
                for (const auto &iter: query_data->at(i)) {
                    queryList->push_back(std::make_shared<KeywordQuery>(termFactory->asFactory(iter)));
                }
                auto boolQuery = BooleanQuery(LogicOperator::WAND, std::move(queryList));
                TopDocs topDocs = searcher.search(boolQuery, K);
                results->at(i) = topDocs;

            }
            double searchCostS = sw2.getElapsedSec();
            double searchCostMs = sw2.getElapsedMs();
            LOG(Helper::LogLevel::LL_Info,
                "Finish searching in %.6lf s, query number=%d, mean latency=%.6lf ms\n",
                searchCostS,query_num, searchCostMs/query_num);

            for(auto i=0;i<std::min(query_num,6);i++) {
                for (const auto &iter: query_data->at(i)) {
                    std::cout << iter << " ";
                }
                std::cout << std::endl;
                results->at(i).print();
            }
            if((*config_map)["SearchIndex"]["checktruth"]=="true"){
                float meanmrr=0;
                auto fs=open((*config_map)["SearchIndex"]["truthfile"].c_str(),std::fstream::in);
                auto truth=std::make_unique<std::vector<std::vector<DocId>>>();
                truth->resize(query_num);
                for(auto i=0;i<query_num;i++){
                    for(auto j=0;j<4;j++){
                        DocId x;
                        fs>>x;
                        truth->at(i).push_back(x);
                    }
                }

                for(auto i=0;i<query_num;i++){
                    int minpos = K;
                    for(auto j=0;j<4;j++){
                        DocId x=truth->at(i).at(j);
                        for(auto l=0;l<results->at(i).getDocNum();l++){
                            if(results->at(i).getIdAt(l)==x){
                                if(l<minpos){
                                    minpos=l;
//                                    std::cout<<"1"<<std::endl;
                                }
//                                std::cout<<"2"<<std::endl;
                                break;
                            }
                        }
                    }
                    if (minpos < K) meanmrr += 1.0f / (minpos + 1);
                }
                meanmrr/=query_num;
                LOG(Helper::LogLevel::LL_Info,
                    "mean mrr=%.6lf \n",
                    meanmrr);
            }


        }
    }

}

BOOST_AUTO_TEST_SUITE(IVFTest)

    BOOST_AUTO_TEST_CASE(IVFTermUpdateSearch)
    {
        std::string test_path = "IVFtest.ini";
        IVF::test_term(test_path);
    }

BOOST_AUTO_TEST_SUITE_END()