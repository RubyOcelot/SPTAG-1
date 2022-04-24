#include <map>
#include <memory>
#include "inc/IVF/DefaultTermIndexConfig.h"
#include "inc/IVF/IndexSearcher.h"
#include "inc/IVF/interfaces/ScoreScheme.h"
#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/utils/TierTree.h"
#include "inc/IVF/utils/RocksDBIO.h"
#include "inc/IVF/utils/LoadTermData.h"

namespace IVF {

    std::unique_ptr<Keyword> IVF::DefaultTermIndexConfig::getFactory() {
        return getTermFactory();
    }

    std::unique_ptr<Term> IVF::DefaultTermIndexConfig::getTermFactory() {
        return std::make_unique<Term>("",termIndex);
    }

    void DefaultTermIndexConfig::init(const std::string &path, IndexSearcher &searcher) {
        Helper::IniReader iniReader;
        iniReader.LoadIniFile(path);
        auto config_map=new std::map<std::string, std::map<std::string, std::string>>;
        (*config_map)["Base"] = iniReader.GetParameters("Base");
        (*config_map)["BuildIndex"] = iniReader.GetParameters("BuildIndex");


        termIndex=std::make_shared<TermIndex>();

        std::unique_ptr<ScoreScheme> scoreScheme;
        if((*config_map)["Base"]["scorescheme"]=="TF-IDF"){
            scoreScheme=std::make_unique<TermTFIDFScoreScheme>();
        }

        auto headIndexFile=(*config_map)["Base"]["headindexfile"];
        if((*config_map)["Base"]["headindextype"]=="TierTree"){
            termIndex->setHeadIndex(std::make_unique<TierTree>(scoreScheme->getEmptyKeywordStatistic(),headIndexFile));
        }
        termIndex->setScoreScheme(std::move(scoreScheme));

        std::unique_ptr<KeyValueIO> kvio;
        if((*config_map)["Base"]["kviotype"]=="Rocksdb"){
            kvio = std::make_unique<RocksDBIO>();
        }
        kvio->Initialize((*config_map)["Base"]["kviopath"].c_str());
        termIndex->setKV(std::move(kvio));

        if((*config_map)["BuildIndex"]["buildindex"]=="true"){
            if((*config_map)["BuildIndex"]["warmuphead"]=="true"){
                auto warmUpHeadIndexFile=(*config_map)["BuildIndex"]["warmupheadindexfile"];
                termIndex->loadHeadIndexWarmup(warmUpHeadIndexFile);
            }
            auto sourceFile=(*config_map)["BuildIndex"]["sourcefile"];
            auto threadNum=std::atoi((*config_map)["BuildIndex"]["numberofthreads"].c_str());
            auto termDataLoader=new LoadTermData();
            termIndex->buildIndex(termDataLoader->getTermSetData(sourceFile, <#initializer#>), threadNum);
        }
        else{
            termIndex->loadHeadIndex(headIndexFile);
        }


        searcher.indexCollection.push_back(termIndex);
        delete config_map;
    }

    void DefaultTermIndexConfig::close() {
        //TODO index lifetime
    }
}
