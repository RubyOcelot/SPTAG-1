#include "inc/IVF/DefaultVectorIndexConfig.h"
#include "inc/IVF/KeyVector.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include "inc/IVF/RocksDbKeyValueIO.h"
#include "inc/Core/VectorIndex.h"

#include "inc/Helper/CommonHelper.h"
#include "inc/SSDServing/main.h"
#include "inc/SSDServing/Utils.h"
#include "inc/SSDServing/SSDIndex.h"

#include "inc/IVF/SSDServingWrapper.h"
#include "inc/IVF/VectorIndexWrapper.h"
using namespace SPTAG;

using namespace SPTAG::SSDServing;

namespace IVF {
    template<typename T>
    void DefaultVectorIndexConfig<T>::init(std::string dir) {

        std::string configFilePath="/home/v-liweiluo/runSPTAG/kvsearch.ini";

        std::map<std::string, std::map<std::string, std::string>> my_map;
        auto vecIndex= IVF::SetupIndex(false, &my_map, configFilePath.c_str());


        auto SPANNIndex=new VectorIndexWrapper<T>(vecIndex);
        KeyVector<T>::vectorIndexWrapper=SPANNIndex;

//        //
//        std::string rocksdb_dir = dir + "//rocksdb";
//        auto kvIO = new RocksDbKeyValueIO();
//        if (kvIO->init(rocksdb_dir)) {
//            KeyVector<T>::keyValueIO = kvIO;
//        } else {
//            //TODO ERROR
//            std::cout << "db init error" << std::endl;
//            std::cout << rocksdb_dir << std::endl;
//        }


        auto collectionStatHolder = new DefaultVectorScoreScheme<T>();
        //TODO load cstat
        int dim=SPANNIndex->getVecLen();
        collectionStatHolder->collectionStatisticsLoader(std::to_string(dim));
        KeyVector<T>::setCollectionStatHolder(collectionStatHolder);
    }

    template<typename T>
    void DefaultVectorIndexConfig<T>::close() {
        KeyVector<T>::vectorIndexWrapper->~VectorIndexWrapper();
    }

    template class DefaultVectorIndexConfig<float>;
    template class DefaultVectorIndexConfig<int8_t>;
}