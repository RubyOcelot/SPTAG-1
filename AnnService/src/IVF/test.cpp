
#include <iostream>
#include "inc/IVF/api.h"
#include <memory>
//#include "postingItem.pb.h"
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

void test_float(){
    std::string test_dir="/tmp/dbtestfloat";
//    test_in(test_dir);
    IndexSearcher searcher=IndexSearcher(test_dir);
    auto temp_v=new float[100];
    for(int i=0;i<100;i++){
        *(temp_v+i)=10;
    }
    auto kv=KeyVector<int8_t>(temp_v);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<float>( std::make_shared<DistanceUtilsWrap<float>>(SPTAG::DistCalcMethod::L2));
    KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
    TopDocs topDocs=searcher.search(kwQuery,3);
    topDocs.print();
}

void test_int8(std::string test_dir){
//    test_in(test_dir);
    IndexSearcher searcher=IndexSearcher(test_dir);
    auto temp_v=new int8_t[100];
    for(int i=0;i<100;i++){
        *(temp_v+i)=10;
    }
    auto kv=KeyVector<int8_t>(temp_v);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<int8_t>( std::make_shared<DistanceUtilsWrap<int8_t>>(SPTAG::DistCalcMethod::L2));
    KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
    TopDocs topDocs=searcher.search(kwQuery,10);
    topDocs.print();
}
int main(int argc, char* argv[]) {
//    test_float();
    if (argc < 2)
    {
        LOG(Helper::LogLevel::LL_Error,
            "configFilePath\n");
        exit(-1);
    }
    std::string test_dir=std::string(argv[1]);
    test_int8(test_dir);
    return 0;
}
