
#include <iostream>
#include "inc/IVF/api.h"
//#include "postingItem.pb.h"

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
    std::vector<float> temp_v;
    for(int i=0;i<100;i++){
        temp_v.push_back(1.0);
    }
//    auto vecValue=std::make_shared<std::vector<float>>(std::vector<float>({4,5,6}));
    auto vecValue=std::make_shared<std::vector<float>>(temp_v);
    KeyVector<float> kv=KeyVector<float>(vecValue);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<float>( std::make_shared<L2DistanceFunction<float>>(L2DistanceFunction<float>()));
    KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
    TopDocs topDocs=searcher.search(kwQuery,3);
    topDocs.print();
}

void test_int8(){
    std::string test_dir="/tmp/dbtestint8";
//    test_in(test_dir);
    IndexSearcher searcher=IndexSearcher(test_dir);
    std::vector<int8_t> temp_v;
    for(int i=0;i<100;i++){
        temp_v.push_back(1);
    }
//    auto vecValue=std::make_shared<std::vector<int8_t>>(std::vector<int8_t>({4,5,6}));
    auto vecValue=std::make_shared<std::vector<int8_t>>(temp_v);
    KeyVector<int8_t> kv=KeyVector<int8_t>(vecValue);
    ScoreScheme* vScoreScheme=new DefaultVectorScoreScheme<int8_t>( std::make_shared<L2DistanceFunction<int8_t>>(L2DistanceFunction<int8_t>()));
    KeywordQuery kwQuery=KeywordQuery(kv, vScoreScheme);
    TopDocs topDocs=searcher.search(kwQuery,3);
    topDocs.print();
}
int main() {
//    test_float();
    test_int8();
    return 0;
}
