
#include <iostream>
#include "inc/IVF/api.h"
#include <memory>
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include <new>

using namespace IVF;

//void test_float(){
//    std::string test_dir="/tmp/dbtestfloat";
//    IndexSearcher searcher=IndexSearcher(test_dir);
//    auto temp_v=new float[100];
//    for(int i=0;i<100;i++){
//        *(temp_v+i)=10;
//    }
//    auto kv=KeyVector(temp_v);
//    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<float>( std::make_shared<DistanceUtilsWrap<float>>(SPTAG::DistCalcMethod::L2));
//    KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
//    TopDocs topDocs=searcher.search(kwQuery,3);
//    topDocs.print();
//}

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

int compare_result(TopDocs& td,std::set<SizeType> truth){
    int miss=0;
    for(auto iter:td.value){
        if(truth.find(iter.docId)==truth.end()){
            miss++;
        }
    }
    return miss;
}

void test_int8(const std::string& test_dir){
    auto opts=getSPTAGOptions(test_dir.c_str());

    auto fullVectors= new VectorReaderWrap;
    fullVectors->loadFullVectorData(opts);

    auto querys=new VectorReaderWrap;
    querys->loadQueryData(opts);

    auto truth=new TruthWrap;
    truth->loadData(opts,querys->numVectors);

    auto searcher=IndexSearcher(test_dir);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<int8_t>( std::make_shared<DistanceUtilsWrap<int8_t>>(SPTAG::DistCalcMethod::L2));

//    std::cout<<opts->m_vectorSize<<" "<<fullVectors->numVectors<<std::endl;
//    for(auto j=opts->m_vectorSize;j<fullVectors->numVectors;j++){
//        auto kv=KeyVector(fullVectors->p_vectorSet->GetVector(j));
//        searcher.addKeyword(kv);
//    }

    int step=fullVectors->numVectors-opts->m_vectorSize;
    int finishedInsert = 0;
    int insertThreads = opts->m_insertThreadNum;
    int curCount=opts->m_vectorSize;

    LOG(Helper::LogLevel::LL_Info, "Start updating...\n");

    StopWSPFresh sw;

    std::vector<std::thread> threads;

    std::atomic_size_t vectorsSent(0);

    auto func = [&]()
    {
        size_t index = 0;
        while (true)
        {
            index = vectorsSent.fetch_add(1);
            if (index < step)
            {
                if ((index & ((1 << 14) - 1)) == 0)
                {
                    LOG(Helper::LogLevel::LL_Info, "Sent %.2lf%%...\n", index * 100.0 / step);
                }

                auto kv=KeyVector(fullVectors->p_vectorSet->GetVector(index+curCount));
                searcher.addKeyword(kv);
            }
            else
            {
                return;
            }
        }
    };
    for (int j = 0; j < insertThreads; j++) { threads.emplace_back(func); }
    for (auto& thread : threads) { thread.join(); }

    double sendingCost = sw.getElapsedSec();
    LOG(Helper::LogLevel::LL_Info,
        "Finish sending in %.3lf seconds, sending throughput is %.2lf , insertion count %u.\n",
        sendingCost,
        step/ sendingCost,
        static_cast<uint32_t>(step));

    while(!searcher.updateAllFinished())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    double syncingCost = sw.getElapsedSec();
    LOG(Helper::LogLevel::LL_Info,
        "Finish syncing in %.3lf seconds, actuall throughput is %.2lf, insertion count %u.\n",
        syncingCost,
        step / syncingCost,
        static_cast<uint32_t>(step));

    curCount += step;
    finishedInsert += step;
    LOG(Helper::LogLevel::LL_Info, "Total Vector num %d \n", curCount);

//        p_index->ForceCompaction();


    //TODO parallel
    float miss_rate=0;
    int s=0;
    int e=1000;
    for(int j=s;j<e;j++){
        auto kv=KeyVector(querys->p_vectorSet->GetVector(j));
        KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
        TopDocs topDocs=searcher.search(kwQuery,truth->truthK);
//        truth->print_truth_by_id(j);
//        topDocs.print_id_sort();
//        topDocs.print();
        int miss= compare_result(topDocs,truth->truth.at(j));
//        if(miss>0){
//            std::cout<<j<<" miss "<<miss<<std::endl;
//        }
        miss_rate+=(float)miss/(float)(truth->truthK);
    }
    //TODO calc recall
    std::cout<<"miss rate: "<<miss_rate/(e-s)<<std::endl;
}

void utils_test(const std::string& test_dir){
    auto opts=getSPTAGOptions(test_dir.c_str());
    auto baseVectors= new VectorReaderWrap;
    baseVectors->loadVectorData(opts);
    auto querys=new VectorReaderWrap;
    querys->loadQueryData(opts);
    auto truth=new TruthWrap;
    truth->loadData(opts,querys->numVectors);
    std::cout << "numBaseVectors:" << baseVectors->numVectors << std::endl;
    std::cout << "numQuerys:" << querys->numVectors << std::endl;
    querys->print_vector_by_id(0);
//    truth->print_truth_by_id(0);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        LOG(Helper::LogLevel::LL_Error,
            "configFilePath\n");
        exit(-1);
    }
    std::string test_dir=std::string(argv[1]);
    test_int8(test_dir);
//    utils_test(test_dir);
    return 0;
}
