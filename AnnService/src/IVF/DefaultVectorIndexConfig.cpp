#include "inc/IVF/DefaultVectorIndexConfig.h"
#include "inc/IVF/KeyVector.h"
#include "inc/IVF/DefaultVectorScoreScheme.h"
#include "inc/Core/VectorIndex.h"

#include "inc/Helper/CommonHelper.h"
#include "inc/SSDServing/main.h"
#include "inc/SSDServing/Utils.h"
#include "inc/SSDServing/SSDIndex.h"

#include "inc/IVF/SPTAGSSDServingWrapper.h"
#include "inc/IVF/VectorIndexWrapper.h"
using namespace SPTAG;

using namespace SPTAG::SSDServing;

namespace IVF {
    void DefaultVectorIndexConfig::init(const std::string &dir, IndexSearcher &searcher) {

        //TODO
        std::string SPTAGConfigFilePath=dir;

        std::map<std::string, std::map<std::string, std::string>> my_map;
        auto vecIndex= IVF::SetupSPTAGIndex( &my_map, SPTAGConfigFilePath.c_str());

        KeyVector::vectorIndexWrapper=std::make_shared<VectorIndexWrapper>(vecIndex);

        int dim=KeyVector::vectorIndexWrapper->getVecLen();
#define DefineVectorValueType(Name, Type) \
                if (vecIndex->GetVectorValueType() == VectorValueType::Name) { \
                    auto collectionStatHolder = new DefaultVectorScoreScheme<Type>();\
                    collectionStatHolder->collectionStatisticsLoader(std::to_string(dim));\
                    KeyVector::setCollectionStatHolder(collectionStatHolder);\
                } \


#include "inc/Core/DefinitionList.h"

#undef DefineVectorValueType
        searcher.indexCollection.push_back(KeyVector::vectorIndexWrapper);
    }

    void DefaultVectorIndexConfig::close() {
        KeyVector::vectorIndexWrapper->~VectorIndexWrapper();
    }

}