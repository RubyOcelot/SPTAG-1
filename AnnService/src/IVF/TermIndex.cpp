#include "inc/IVF/TermIndex.h"
#include "inc/IVF/utils/TierTree.h"

namespace IVF{

    bool TermIndex::updateAllFinished() {
        return false;
    }

    void TermIndex::close() {

    }

    void
    TermIndex::buildIndex(std::unique_ptr<TermSetDataHolder> dataHolder, int threadNum, const std::string& headIndexFile) {
        collectionStatistic=std::move(dataHolder->cstat);
        //TODO parallel;
        for(auto i=0;i<dataHolder->termNum;i++){
            auto &data= dataHolder->termDataVec.at(i);
            auto hid=head_index->set(data.str,data.kwstat->getContent());
            if(hid==-1){
                //TODO error
                exit(1);
            }
            setPostingList(hid,data.posting_data);
        }
        auto fs=std::make_unique<std::fstream>();
        fs->open(headIndexFile,std::fstream::binary);
        head_index->storeIndex(std::move(fs));
    }

    void TermIndex::setPostingList(HeadIDType headID, const std::string &value) {
        if(db == nullptr){
            //TODO error
            std::cout<<"db=null!"<<std::endl;
            exit(1);
        }
        //TODO ErrorCode;
        db->Put(headID,value);
    }

    HeadIDType TermIndex::getHeadID(const std::string &str, std::string *rt_stat) {
        if(head_index == nullptr){
            //TODO error
        }
        return head_index->seek(str, rt_stat);
    }

    void TermIndex::getPostingList(HeadIDType headID,std::istream **value) {
        if(db == nullptr){
            //TODO error
        }
        //TODO ErrorCode;
        db->Get(headID,value);
    }

    void TermIndex::setHeadIndex(std::unique_ptr<StringHeadIndex> head_index) {
        this->head_index=std::move(head_index);
    }

    void TermIndex::setKV(std::unique_ptr<KeyValueIO> kvio) {
        this->db=std::move(kvio);
    }

    void TermIndex::loadHeadIndex(const std::string& path) {
        auto fs=std::make_unique<std::fstream>();
        fs->open(path,std::fstream::binary);
        head_index->loadIndex(std::move(fs));

    }

    void TermIndex::loadHeadIndexWarmup(const std::string& path) {
        auto fs=std::make_unique<std::fstream>();
        fs->open(path,std::fstream::binary);
        head_index->loadWarmupIndex(std::move(fs));
    }

    void TermIndex::setScoreScheme(std::unique_ptr<ScoreScheme> scoreScheme) {
        this->scoreScheme=std::move(scoreScheme);
    }

    std::unique_ptr<ScoreScheme> TermIndex::getCollectionStatHolder() {
        auto s=scoreScheme->smart_clone();
        s->collectionStatisticsLoader(collectionStatistic->getContent());
        return s;
    }
}