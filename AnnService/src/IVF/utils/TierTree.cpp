#include <utility>

#include "inc/IVF/utils/TierTree.h"
#include "inc/IVF/utils/DataTransform.h"

namespace IVF{
    TierTree::Children::Children() {
        charIndex=std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[sizeof(char)*0x100]);
        for(auto i=0;i<sizeof(char)*0x100;i++){
            charIndex[i]=nullptr;
        }
        charRWLock=std::shared_ptr<std::shared_mutex[]>(new std::shared_mutex[sizeof(char)*0x100]);
    }

    TierTree::Children::~Children() {
    }

    std::shared_ptr<TierTree::Node> TierTree::Children::getChild(const std::string &str, int curPos) const {
        unsigned char fc=str[curPos];
        charRWLock[fc].lock_shared();
        auto node=charIndex[fc];
        charRWLock[fc].unlock_shared();
        return node;
    }

    std::shared_ptr<TierTree::Node> TierTree::Children::getOrAddChild(const std::string &str, int curPos, std::atomic<HeadIDType>& curHeadId_ref,
                                                                      const std::unique_ptr<KeywordStatistic> &stat) {
        unsigned char fc=str[curPos];
        charRWLock[fc].lock_shared();
        auto node=charIndex[fc];
        charRWLock[fc].unlock_shared();
        if(node!=nullptr)
            return node;

        charRWLock[fc].lock();
        node=charIndex[fc];
        if(node==nullptr) {
            auto new_str=str.substr(curPos,std::string::npos);
            auto newStat= stat->getNew();
            node=std::make_shared<Node>(new_str, curHeadId_ref,std::move(newStat));
            curHeadId_ref++;
            charIndex[fc]=node;
        }
        charRWLock[fc].unlock();
        return node;
    }

    void TierTree::Children::addChild(const std::shared_ptr<Node>& child) {

        //for a new node child, where nobody got its divergeLock, won't freeze;
        child->divergeLock.lock_shared();
        unsigned char fc=child->identity[0];
        child->divergeLock.unlock_shared();

        charRWLock[fc].lock();
        charIndex[fc]=child;
        charRWLock[fc].unlock();
    }

    TierTree::TierTree(std::unique_ptr<KeywordStatistic> emptyStat, std::string filePath) : curHeadId(0),filePath(std::move(filePath)){

        kwStatTemplate=emptyStat->getNew();
        root=(std::make_shared<Node>("", 0, std::move(emptyStat)));
        if(root->stat== nullptr){
            //TODO error
        }
        curHeadId++;
    }


    HeadIDType TierTree::seek(const std::string &str, std::string* rt_stat) {
        auto node = seekInternalNoInsert(str, root, 0);
        if(node == nullptr) {
            return -1;
        }
        else {
            if (rt_stat != nullptr) {
                *rt_stat = node->stat->getContent();
            }
            auto hid = node->headID;
            node->divergeLock.unlock_shared();
            return hid;
        }
        return -1;
    }

    HeadIDType TierTree::set(const std::string &str, std::string stat) {
        auto node = seekInternalWithInsert(str, root, 0);
        node->stat->set(stat);
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::add(const std::string &str, std::string inputStat) {
        auto node = seekInternalNoInsert(str, root, 0);
        if(node == nullptr) {
            //TODO error;
            exit(1);
            return -1;
        }
        else {
            node->stat->modForAdd(inputStat);
            auto hid = node->headID;
            node->divergeLock.unlock_shared();
            return hid;
        }
        return -1;
    }

    HeadIDType TierTree::del(const std::string &str, std::string inputStat) {
        auto node = seekInternalNoInsert(str, root, 0);
        if(node == nullptr) {
            //TODO error;
            exit(1);
            return -1;
        }
        else {
            node->stat->modForDel(inputStat);
            auto hid = node->headID;
            node->divergeLock.unlock_shared();
            return hid;
        }
        return -1;
    }

    std::shared_ptr<TierTree::Node> TierTree::seekInternalWithInsert(const std::string &str, std::shared_ptr<TierTree::Node> curNode, int curPos) {
        if(curNode==nullptr)
            return nullptr;
        curNode->divergeLock.lock_shared();
        auto isPref= isPrefix(curNode->identity, str, curPos);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            auto child=children->getOrAddChild(str, curPos, curHeadId, curNode->stat);
            curNode->divergeLock.unlock_shared();
            auto ret_val=seekInternalWithInsert(str, child, curPos);
            return ret_val;
        }

        curNode->divergeLock.unlock_shared();
        //regain w lock
        curNode->divergeLock.lock();
        isPref= isPrefix(curNode->identity, str, curPos);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                curNode->divergeLock.unlock_and_lock_shared();
                return curNode;
            }
            auto children=curNode->children;
            auto child=children->getOrAddChild(str, curPos, curHeadId, curNode->stat);
            curNode->divergeLock.unlock();
            return seekInternalWithInsert(str, child, curPos);
        }

        auto newPos= checkPrefix(str.substr(curPos,std::string::npos),curNode->identity);
        auto newChild1=std::make_shared<Node>(curNode->identity.substr(newPos,std::string::npos),curNode->headID,std::move(curNode->stat),curNode->children);
        curNode->identity=curNode->identity.substr(0,newPos);
        curNode->headID=curHeadId++;
        curNode->stat=newChild1->stat->getNew();
        curNode->children.reset(new Children());
        curNode->children->addChild(newChild1);
        if(newPos!=str.length()){
            auto newIdentity=str.substr(curPos + newPos,std::string::npos);
            auto newChild2=std::make_shared<Node>(newIdentity, curHeadId++, curNode->stat->getNew());
            newChild2->divergeLock.lock_shared();
            curNode->children->addChild(newChild2);

            curNode->divergeLock.unlock();
            return newChild2;
        }
        curNode->divergeLock.unlock_and_lock_shared();
        return curNode;
    }


    std::shared_ptr<TierTree::Node> TierTree::seekInternalNoInsert(const std::string &str, std::shared_ptr<TierTree::Node> curNode, int curPos) {
         if(curNode==nullptr)
            return nullptr;
        curNode->divergeLock.lock_shared();
        auto isPref= isPrefix(curNode->identity, str, curPos);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            curNode->divergeLock.unlock_shared();
            auto child=children->getChild(str, curPos);
            return seekInternalNoInsert(str, child, curPos);
        }
        return nullptr;
    }

    bool TierTree::isPrefix(const std::string &prefix, const std::string &full, int curPos) {
        if(prefix.length()+curPos>full.length())
            return false;
        return prefix==full.substr(curPos, prefix.length());
    }

    int TierTree::checkPrefix(const std::string &a, const std::string &b) {
        for(auto i=0;i<min(a.length(),b.length());i++){
            if(a[i]!=b[i]){
                return i;
            }
        }
        return 0;
    }

    void TierTree::loadIndex(std::unique_ptr<std::istream> data) {
        root=LoadNode(std::move(data),false);
    }

    void TierTree::loadWarmupIndex(std::unique_ptr<std::istream> data) {
        root=LoadNode(std::move(data),true);
    }


    std::shared_ptr<TierTree::Node> TierTree::LoadNode(const std::shared_ptr<std::istream>& loadStream, bool warmUp) {
        auto curNode=Node::deserialize(loadStream,kwStatTemplate->getNew(),warmUp);
        int buffer_size=10;
        char buffer[buffer_size+1];
        loadStream->read((char*)(buffer), 2);
        u_int16_t child_len=*((u_int16_t*)buffer);
        for(auto i=0;i<child_len;i++){
            auto childNode= LoadNode(loadStream,warmUp);
            curNode->children->addChild(childNode);
        }
        return curNode;
    }

    void TierTree::storeIndex(std::unique_ptr<std::ostream> storeStream) {
        storeNode(std::move(storeStream),root);

    }

    void TierTree::storeNode(const std::shared_ptr<std::ostream>& storeStream, const std::shared_ptr<TierTree::Node>& curNode) {
        auto nodeData=curNode->serialize();
        (*storeStream)<<nodeData;
        u_int16_t child_count=0;
        for(auto i=0;i<sizeof(char)*0x100;i++){
            if(curNode->children->charIndex[i]!= nullptr){
                child_count++;
            }
        }
        (*storeStream)<<DataToString(child_count);
        for(auto i=0;i<sizeof(char)*0x100;i++){
            if(curNode->children->charIndex[i]!= nullptr){
                storeNode(storeStream,curNode->children->charIndex[i]);
            }
        }

    }



    std::string TierTree::Node::serialize() {
        auto statData=stat->getContent();
        return DataToString(headID)+DataToString(identity.length())+identity+DataToString(statData.length())+statData;
    }

    std::shared_ptr<TierTree::Node>
    TierTree::Node::deserialize(const std::shared_ptr<std::istream>& data, std::unique_ptr<KeywordStatistic> kwStatTemplate, bool warmUp) {
        int buffer_size=1000;
        char buffer[buffer_size+1];

        auto node=std::make_shared<Node>();

        data->read((char*)(buffer), sizeof(HeadIDType));
        node->headID=*((HeadIDType*)buffer);

        data->read((char*)(buffer), 4);
        int id_len=*((u_int32_t*)buffer);
        data->read((char*)(buffer), id_len);
        buffer[id_len]='\0';
        node->identity=std::string(buffer);

            data->read((char *) (buffer), 4);
            int stat_len = *((u_int32_t *) buffer);
            data->read((char *) (buffer), stat_len);
            buffer[stat_len] = '\0';
            node->stat = kwStatTemplate->getNew();
        if(!warmUp) {
            node->stat->set(std::string(buffer));
        }

        return node;
    }


}