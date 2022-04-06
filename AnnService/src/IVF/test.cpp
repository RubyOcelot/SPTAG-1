
#include <iostream>
#include "inc/IVF/api.h"
#include <memory>
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include <new>

using namespace IVF;

//void test_in(std::string dir){
//    std::string rocksdb_dir=dir+"//rocksdb";
//    auto kvIO=new RocksDbKeyValueIO();
//    std::cout<<kvIO->init(rocksdb_dir)<<std::endl;
//    auto key_list=std::vector<std::string>({"0","1","2"});
//    auto value_list=std::vector<std::string>(3);
//    pb::postingItem x[3];
//    x[0].set_docid(2);
//    x[1].set_docid(4);
//    x[2].set_docid(8);
//    for(int i=0;i<3;i++){
//        for(int j=0;j<3;j++){
//            x[i].add_vecval(j+i*3+1);
//        }
//        x[i].SerializeToString(&value_list[i]);
//    }
//
//    for(int i=0;i<3;i++){
//        kvIO->set(key_list[i],value_list[i]);
//    }
//    kvIO->shutdown();
//}

//void test_float(){
//    std::string test_dir="/tmp/dbtestfloat";
////    test_in(test_dir);
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
    auto querys=new QuerysWrap;
    querys->loadData(opts);

    auto truth=new TruthWrap;
    truth->loadData(opts,querys->numQueries);

    auto searcher=IndexSearcher(test_dir);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<int8_t>( std::make_shared<DistanceUtilsWrap<int8_t>>(SPTAG::DistCalcMethod::L2));

    //TODO parallel
    float miss_rate=0;
    int s=0;
    int e=10000;
    for(int j=s;j<e;j++){
        auto kv=KeyVector(querys->querySet->GetVector(j));
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

int utils_test(const std::string& test_dir){
    auto opts=getSPTAGOptions(test_dir.c_str());
    auto querys=new QuerysWrap;
    querys->loadData(opts);
    auto truth=new TruthWrap;
    truth->loadData(opts,querys->numQueries);
    std::cout<<"numQuerys:"<<querys->numQueries<<std::endl;
    querys->print_vector_by_id(0);
    truth->print_truth_by_id(0);
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
