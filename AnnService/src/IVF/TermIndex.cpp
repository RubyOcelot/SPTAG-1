#include "inc/IVF/TermIndex.h"
#include "inc/IVF/utils/TierTree.h"

namespace IVF{

    bool TermIndex::updateAllFinished() {
        return false;
    }

    void TermIndex::close() {

    }

    void TermIndex::buildIndex(std::unique_ptr<TermSetDataHolder> dataHolder,int threadNum) {
        collectionStatistic=std::move(dataHolder->cstat);
        //TODO parallel;
        for(auto i=0;i<dataHolder->termNum;i++){
            auto data=dataHolder->termData+i;
            auto hid=head_index->set(data->term.getStr(),data->kwstat->getContent());
            setPostingList(hid,data->posting_data);
        }
    }

    void TermIndex::setPostingList(HeadIDType headID, const std::string &value) {
        if(db == nullptr){
            //TODO error
        }
        //TODO ErrorCode;
        db->Put(headID,value);
    }

    HeadIDType TermIndex::getHeadID(const Term &term, std::string *rt_stat) {
        if(head_index == nullptr){
            //TODO error
        }
        return head_index->seek(term.getStr(),rt_stat);
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

    }

    void TermIndex::loadHeadIndexWarmup(const std::string& path) {

    }
}