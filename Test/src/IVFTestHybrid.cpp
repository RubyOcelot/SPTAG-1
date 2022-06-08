

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


    void test_update(Options *opts, IndexSearcher &searcher, std::shared_ptr<KeyVector> kvFactory) {
        auto fullVectors = VectorReaderWrap();
        fullVectors.loadFullVectorData(opts);
        int step = fullVectors.numVectors - opts->m_vectorSize;
        int insertThreads = opts->m_insertThreadNum;
        int curCount = opts->m_vectorSize;

        LOG(Helper::LogLevel::LL_Info, "Start updating...\n");

        StopWSPFresh sw;

        std::vector<std::thread> insert_threads;

        std::atomic_size_t vectorsSent(0);

        auto func = [&]() {
            size_t index = 0;
            while (true) {
                index = vectorsSent.fetch_add(1);
                if (index < step) {
                    if ((index & ((1 << 14) - 1)) == 0) {
                        LOG(Helper::LogLevel::LL_Info, "Sent %.2lf%%...\n", index * 100.0 / step);
                    }

                    searcher.addKeyword(kvFactory->asFactory(fullVectors.p_vectorSet->GetVector(index + curCount)));
                } else {
                    return;
                }
            }
        };
        for (int j = 0; j < insertThreads; j++) { insert_threads.emplace_back(func); }
        for (auto &thread: insert_threads) { thread.join(); }

        double sendingCost = sw.getElapsedSec();
        LOG(Helper::LogLevel::LL_Info,
            "Finish sending in %.3lf seconds, sending throughput is %.2lf , insertion count %u.\n",
            sendingCost,
            step / sendingCost,
            static_cast<uint32_t>(step));

        while (!searcher.updateAllFinished()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        double syncingCost = sw.getElapsedSec();
        LOG(Helper::LogLevel::LL_Info,
            "Finish syncing in %.3lf seconds, actuall throughput is %.2lf, insertion count %u.\n",
            syncingCost,
            step / syncingCost,
            static_cast<uint32_t>(step));

        curCount += step;
        LOG(Helper::LogLevel::LL_Info, "Total Vector num %d \n", curCount);
    }

    template<class Type>
    float CalculateRecallIVF(const std::shared_ptr<std::vector<TopDocs>>& results, TruthWrap &truth, SPTAG::DistCalcMethod distCalcMethod, VectorReaderWrap& vectorSet, VectorReaderWrap &querys, Options *opts){


        auto NumQuerys=results->size();
        auto K=truth.truthK;

        float meanrecall = 0, minrecall = MaxDist, maxrecall = 0, stdrecall = 0;
        std::vector<float> thisrecall(NumQuerys, 0);
        std::unique_ptr<bool[]> visited(new bool[K]);
        for (SizeType i = 0; i < NumQuerys; i++)
        {
            memset(visited.get(), 0, K * sizeof(bool));
            for (SizeType id : truth.truth[i])
            {
                for (int j = 0; j < results->at(i).getDocNum(); j++)
                {
                    if (visited[j] ) continue;

                    //TODO may change later
                    float dist = results->at(i).value->at(j).score*(-1);
                    float truthDist;

                    truthDist = COMMON::DistanceUtils::ComputeDistance((const Type*)querys.p_vectorSet->GetVector(i), (const Type*)vectorSet.p_vectorSet->GetVector(id), vectorSet.p_vectorSet->Dimension(), distCalcMethod);


                    if ((distCalcMethod == SPTAG::DistCalcMethod::Cosine) && (std::fabs(dist - truthDist) < Epsilon)) {
                        thisrecall[i] += 1;
                        visited[j] = true;
                        break;
                    }
                    else if ((distCalcMethod == SPTAG::DistCalcMethod::L2) && (std::fabs(dist - truthDist) < Epsilon * (dist + Epsilon))) {
                        thisrecall[i] += 1;
                        visited[j] = true;
                        break;
                    }

                }
            }
            thisrecall[i] /= (float)truth.truthK;
            meanrecall += thisrecall[i];
            if (thisrecall[i] < minrecall) minrecall = thisrecall[i];
            if (thisrecall[i] > maxrecall) maxrecall = thisrecall[i];

        }
        meanrecall /= (float)NumQuerys;
        for (SizeType i = 0; i < NumQuerys; i++)
        {
            stdrecall += (thisrecall[i] - meanrecall) * (thisrecall[i] - meanrecall);
        }
        stdrecall = std::sqrt(stdrecall / NumQuerys);

        //TODO debug
//        std::cout << meanrecall << " " << stdrecall << " " << minrecall << " " << maxrecall << std::endl;

        return meanrecall;
    }

    void
    test_search(Options *opts, IndexSearcher searcher, std::shared_ptr<KeyVector> kvFactory, VectorReaderWrap &querys,
                TruthWrap &truth, int s, int e, SPTAG::DistCalcMethod distCalcMethod) {
        LOG(Helper::LogLevel::LL_Info, "Start searching...\n");

//        ScoreScheme *vScoreScheme = new DefaultVectorScoreScheme<int8_t>(
//                std::make_shared<DistanceUtilsWrap<int8_t>>(SPTAG::DistCalcMethod::L2));

        int searchThreads = opts->m_searchThreadNum;

        std::vector<std::thread> search_threads;

        std::atomic_size_t querysSent(s);

        std::shared_ptr<std::vector<TopDocs>> results=std::make_shared<std::vector<TopDocs>>();
        results->resize(e-s);

        auto func2 = [&]() {
            size_t index = 0;
            while (true) {
                index = querysSent.fetch_add(1);
                if (index < e) {
                    if ((index & ((1 << 12) - 1)) == 0) {
                        LOG(Helper::LogLevel::LL_Info, "Sent %.2lf%%...\n", index * 100.0 / (e - s));
                    }

                    KeywordQuery kwQuery = KeywordQuery(kvFactory->asFactory(querys.p_vectorSet->GetVector(index)));
                    TopDocs topDocs = searcher.search(kwQuery, truth.truthK);
//                topDocs.print_id_sort();
//                truth->print_truth_by_id(index);
                    results->at(index) = (topDocs);

                } else {
                    return;
                }
            }
        };

        StopWSPFresh sw;

        for (int j = 0; j < searchThreads; j++) { search_threads.emplace_back(func2); }
        for (auto &thread: search_threads) { thread.join(); }

        double searchingCost = sw.getElapsedSec();


        if(!opts->m_truthPath.empty()){

            StopWSPFresh calc_recall_sw;

            VectorReaderWrap vectorSet;
            if (opts->m_update) {
                vectorSet.loadFullVectorData(opts);
            } else{
                vectorSet.loadVectorData(opts);
            }

            float mean_recall;

#define DefineVectorValueType(Name, Type) \
	if (opts->m_valueType == VectorValueType::Name) { \
        mean_recall=CalculateRecallIVF<Type>(results, truth, distCalcMethod, vectorSet, querys, opts); \
	} \

#include "inc/Core/DefinitionList.h"
#undef DefineVectorValueType

            double calcRecallCost = calc_recall_sw.getElapsedSec();


            LOG(Helper::LogLevel::LL_Info,
                "Finish calc recall in %.3lf seconds, mean recall rate is %.3lf.\n",
                calcRecallCost,
                mean_recall);
        }


        LOG(Helper::LogLevel::LL_Info,
            "Finish searching in %.3lf seconds, searching throughput is %.2lf, searching count %u.\n",
            searchingCost,
            (e - s) / searchingCost,
            static_cast<uint32_t>(e - s));

    }

    class HybridQueryData{
    public:
        std::vector<std::string> terms;
        void* vec;
    };

    std::shared_ptr<std::vector<HybridQueryData>> getQuery(const std::string &path, const SourceDataType &dataType,
                                                                    const int threadNum,const int vecDim){

        int queryNum;
        std::shared_ptr<std::vector<inputData>> inputDataVec=std::make_shared<std::vector<inputData>>();

        std::fstream fs;

        auto mode=std::fstream::in;
        if(dataType==SourceDataType::tsv){
            const int buffer_size=1000000;
            char *buffer=new char[buffer_size];
            fs.open (path, mode);

            queryNum=0;
            while(!fs.eof()){
                DocId docId;
                fs>>docId;
                char c;
                int i=0;
                fs.get(c);
                while(c!='{'){
                    buffer[i++]=c;
                }
                buffer[i]='\0';
                inputDataVec->push_back(inputData(docId,buffer));
                for(auto j=0;j<vecDim;j++){

                }
                queryNum++;
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

//        return ret;

    }

    void test_combine(const std::string &test_dir){
        StopWSPFresh sw1;
        auto indexConfig=std::make_shared<SparseAndDenseIndexConfig>();
        auto searcher = IndexSearcher(test_dir, indexConfig);
        double setupCost = sw1.getElapsedSec();
        LOG(Helper::LogLevel::LL_Info,
            "Finish hybrid index setup in %.3lf seconds\n",
            setupCost);
        std::shared_ptr<KeyVector> kvFactory=indexConfig->getTermAndVectorFactory();
        std::shared_ptr<Term> termFactory=indexConfig->getTermAndVectorFactory();

        Helper::IniReader iniReader;
        iniReader.LoadIniFile(test_dir);
        auto config_map=std::make_unique<std::map<std::string, std::map<std::string, std::string>>>();
        (*config_map)["SearchIndex"] = iniReader.GetParameters("SearchIndex");

        if((*config_map)["SearchIndex"]["searchindex"]=="true") {
            auto sourceFile = (*config_map)["SearchIndex"]["sourcefile"];
            auto sourceFileType = (*config_map)["SearchIndex"]["sourcefiletype"];
            SourceDataType sourceDataType;
            if (sourceFileType == "txt")
                sourceDataType = SourceDataType::txt;
            else if (sourceFileType == "tsv")
                sourceDataType = SourceDataType::tsv;
            else if (sourceFileType == "bin")
                sourceDataType = SourceDataType::bin;
            auto threadNum = std::atoi((*config_map)["SearchIndex"]["numberofthreads"].c_str());

            int K = std::atoi((*config_map)["SearchIndex"]["k"].c_str());
            int vecDim = std::atoi((*config_map)["SearchIndex"]["vecdim"].c_str());
            auto query_data = getQuery(sourceFile, sourceDataType, threadNum, vecDim);

            int query_num = query_data->size();

            std::shared_ptr<std::vector<TopDocs>> results = std::make_shared<std::vector<TopDocs>>();
            results->resize(query_num);
            StopWSPFresh sw2;

            for (auto i = 0; i < query_data->size(); i++) {

//                auto queryList = std::make_unique<std::vector<std::shared_ptr<Query>>>();
//                for (const auto &iter: query_data->at(i)) {
//                    queryList->push_back(std::make_shared<KeywordQuery>(termFactory->asFactory(iter)));
//                }
//                auto boolQuery = BooleanQuery(LogicOperator::WAND, std::move(queryList));
//                TopDocs topDocs = searcher.search(boolQuery, K);
//                results->at(i) = topDocs;

            }
            double searchCostS = sw2.getElapsedSec();
            double searchCostMs = sw2.getElapsedMs();
            LOG(Helper::LogLevel::LL_Info,
                "Finish searching in %.6lf s, query number=%d, mean latency=%.6lf ms\n",
                searchCostS, query_num, searchCostMs / query_num);

//            for(auto i=0;i<std::min(query_num,6);i++) {
//                for (const auto &iter: query_data->at(i)) {
//                    std::cout << iter << " ";
//                }
//                std::cout << std::endl;
//                results->at(i).print();
//            }
        }

    }


}

BOOST_AUTO_TEST_SUITE(IVFTest)

BOOST_AUTO_TEST_CASE(IVFVecUpdateSearch)
        {
            std::string test_path = "IVFtest.ini";
            IVF::test_combine(test_path);
        }

BOOST_AUTO_TEST_SUITE_END()