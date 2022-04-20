#include <map>
#include "inc/IVF/DefaultTermIndexConfig.h"
#include "inc/IVF/IndexSearcher.h"
#include "inc/IVF/interfaces/ScoreScheme.h"
#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/utils/TierTree.h"

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
        if((*config_map)["Base"]["ScoreScheme"]=="TF-IDF"){
            scoreScheme=std::make_unique<TermTFIDFScoreScheme>();
        }

        if((*config_map)["Base"]["HeadIndexType"]=="TierTree"){
            termIndex->setHeadIndex(std::make_unique<TierTree>(scoreScheme->getEmptyKeywordStatistic()));
        }
        auto headIndexFile=(*config_map)["Base"]["HeadIndexFile"];

        searcher.indexCollection.push_back(termIndex);
        delete config_map;
    }

    void DefaultTermIndexConfig::close() {
        //TODO index lifetime
    }
}
