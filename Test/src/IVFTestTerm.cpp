
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


    void test_term(const std::string &test_dir) {
        auto opts = getSPTAGOptions(test_dir.c_str());

        auto indexConfig=std::make_shared<DefaultTermIndexConfig>();

        auto searcher = IndexSearcher(test_dir, indexConfig);

        std::shared_ptr<Term> termFactory=indexConfig->getTermFactory();

        std::vector<std::string> wordList={"mouse","creative"};
        auto queryList=std::make_unique<std::vector<std::shared_ptr<Query>>>();
        for(const auto& iter:wordList){
            queryList->push_back(std::make_shared<KeywordQuery>(termFactory->asFactory(iter)));
        }
        auto boolQuery = BooleanQuery(LogicOperator::OR,std::move(queryList));
        TopDocs topDocs = searcher.search(boolQuery, 5);
        topDocs.print();

        delete opts;
    }

}

BOOST_AUTO_TEST_SUITE(IVFTest)

    BOOST_AUTO_TEST_CASE(IVFTermUpdateSearch)
    {
        std::string test_path = "IVFtest.ini";
        IVF::test_term(test_path);
    }

BOOST_AUTO_TEST_SUITE_END()