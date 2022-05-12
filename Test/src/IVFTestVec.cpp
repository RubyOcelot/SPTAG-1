

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
        std::cout << meanrecall << " " << stdrecall << " " << minrecall << " " << maxrecall << std::endl;

        return meanrecall;
    }

    void
    test_search(Options *opts, IndexSearcher searcher, std::shared_ptr<KeyVector> kvFactory, VectorReaderWrap &querys,
                TruthWrap &truth, int s, int e, SPTAG::DistCalcMethod distCalcMethod) {
        LOG(Helper::LogLevel::LL_Info, "Start searching...\n");

//        ScoreScheme *vScoreScheme = new DefaultVectorScoreScheme<int8_t>(
//                std::make_shared<DistanceUtilsWrap<int8_t>>(SPTAG::DistCalcMethod::L2));

        int searchThreads = opts->m_searchThreadNum;
        StopWSPFresh sw;

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
        for (int j = 0; j < searchThreads; j++) { search_threads.emplace_back(func2); }
        for (auto &thread: search_threads) { thread.join(); }


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

        double searchingCost = sw.getElapsedSec();
        LOG(Helper::LogLevel::LL_Info,
            "Finish searching in %.3lf seconds, searching throughput is %.2lf, searching count %u.\n",
            searchingCost,
            (e - s) / searchingCost,
            static_cast<uint32_t>(e - s));

        //TODO calc true recall
        std::cout << "recall rate: " << mean_recall << std::endl;
    }

    void test_combine(const std::string &test_dir){
        auto opts = getSPTAGOptions(test_dir.c_str());
        auto indexConfig=std::make_shared<SparseAndDenseIndexConfig>();
        auto searcher = IndexSearcher(test_dir, indexConfig);
        std::shared_ptr<KeyVector> kvFactory=indexConfig->getTermAndVectorFactory();
        std::shared_ptr<Term> termFactory=indexConfig->getTermAndVectorFactory();

        delete opts;
    }

    void test_int8(const std::string &test_dir) {
        auto opts = getSPTAGOptions(test_dir.c_str());

        auto indexConfig=std::make_shared<DefaultVectorIndexConfig>();

        auto searcher = IndexSearcher(test_dir, indexConfig);

        std::shared_ptr<KeyVector> kvFactory=indexConfig->getVectorFactory();

        SPTAG::DistCalcMethod distCalcMethod=indexConfig->vectorDistCalcMethod;

        if (opts->m_update) {
            test_update(opts, searcher,kvFactory);
        }

        auto querys = VectorReaderWrap();
        querys.loadQueryData(opts);

        auto truth = TruthWrap();
        truth.loadData(opts, querys.numVectors);

        test_search(opts, searcher, kvFactory, querys, truth, 0, querys.numVectors, distCalcMethod);
        delete opts;
    }

    void utils_test(const std::string &test_dir) {
        auto opts = getSPTAGOptions(test_dir.c_str());
        auto baseVectors = VectorReaderWrap();
        baseVectors.loadVectorData(opts);
        auto querys = VectorReaderWrap();
        querys.loadQueryData(opts);
        auto truth = TruthWrap();
        truth.loadData(opts, querys.numVectors);
        std::cout << "numBaseVectors:" << baseVectors.numVectors << std::endl;
        std::cout << "numQuerys:" << querys.numVectors << std::endl;
        querys.print_vector_by_id(0);
        delete opts;
    }
}

BOOST_AUTO_TEST_SUITE(IVFTest)

BOOST_AUTO_TEST_CASE(IVFVecUpdateSearch)
        {
            std::string test_path = "IVFtest.ini";
            IVF::test_int8(test_path);
//    utils_test(test_dir);
        }

BOOST_AUTO_TEST_SUITE_END()