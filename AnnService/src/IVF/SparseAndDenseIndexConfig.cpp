#include "inc/IVF/SparseAndDenseIndexConfig.h"
#include "inc/IVF/DefaultVectorIndexConfig.h"
#include "inc/IVF/DefaultTermIndexConfig.h"

namespace IVF{

    SparseAndDenseIndexConfig::TermAndVector::TermAndVector(const Term &term, const KeyVector &kv) {
        termIndex=term.termIndex;
        vectorIndexWrapper=kv.vectorIndexWrapper;
    }

    void SparseAndDenseIndexConfig::init(const std::string &path, IndexSearcher &searcher) {
        Helper::IniReader iniReader;
        iniReader.LoadIniFile(path);
        auto config_map=std::make_unique<std::map<std::string, std::map<std::string, std::string>>>();
        (*config_map)["Base"] = iniReader.GetParameters("Base");

        auto vecIndexConfig=DefaultVectorIndexConfig();
        vecIndexConfig.init((*config_map)["Base"]["vecconfigfile"], searcher);
        kv=vecIndexConfig.getVectorFactory();
        auto termIndexConfig=DefaultTermIndexConfig();
        termIndexConfig.init((*config_map)["Base"]["termconfigfile"], searcher);
        term=termIndexConfig.getTermFactory();
    }

    std::unique_ptr<Keyword> SparseAndDenseIndexConfig::getFactory() {
        return (std::unique_ptr<Keyword>) (std::unique_ptr<Term>)(getTermAndVectorFactory());
    }

    void SparseAndDenseIndexConfig::close() {

    }

    std::unique_ptr<SparseAndDenseIndexConfig::TermAndVector> SparseAndDenseIndexConfig::getTermAndVectorFactory() {
        return std::make_unique<TermAndVector>(*term,*kv);
    }
}