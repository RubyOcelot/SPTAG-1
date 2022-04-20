#include "inc/IVF/utils/TierTree.h"

namespace IVF{
    TierTree::Children::Children() {
        charIndex=new std::shared_ptr<Node>[sizeof(char)];
        for(auto i=0;i<sizeof(char);i++){
            charIndex[i]=nullptr;
        }
        charRWLock=new std::shared_mutex[sizeof(char)];
    }

    TierTree::Children::~Children() {
        delete[] charIndex;
        delete[] charRWLock;
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
            node=std::make_shared<Node>(str.substr(curPos,std::string::npos), curHeadId_ref++, stat->getNew());
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

    TierTree::TierTree(std::unique_ptr<KeywordStatistic> emptyStat):curHeadId(0),
                                                                    root(std::make_shared<Node>("", 0, std::move(emptyStat))){
        curHeadId++;
        if(root->stat== nullptr){
            //TODO error
        }
    }


    HeadIDType TierTree::seek(const std::string &str, std::string* rt_stat) {
        auto node = seekInternalNoInsert(str, root, 0);
        if(rt_stat!=nullptr){
            *rt_stat=node->stat->getContent();
        }
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::set(const std::string &str, std::string stat) {
        auto node = seekInternalWithInsert(str, root, 0);
        node->stat->set(stat);
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::add(const std::string &str, std::string inputStat) {
        auto node = seekInternalWithInsert(str, root, 0);
        node->stat->modForAdd(inputStat);
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::del(const std::string &str, std::string inputStat) {
        auto node = seekInternalNoInsert(str, root, 0);
        node->stat->modForDel(inputStat);
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    std::shared_ptr<TierTree::Node> TierTree::seekInternalWithInsert(const std::string &str, std::shared_ptr<TierTree::Node> curNode, int curPos) {
        if(curNode==nullptr)
            return nullptr;
        curNode->divergeLock.lock_shared();
        auto isPref=isPrefix(curNode->identity,str);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            curNode->divergeLock.unlock_shared();
            return seekInternalNoInsert(str, children->getOrAddChild(str, curPos, curHeadId, curNode->stat), curPos);
        }

        curNode->divergeLock.unlock_shared();
        //regain w lock
        curNode->divergeLock.lock();
        isPref=isPrefix(curNode->identity,str);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                curNode->divergeLock.unlock_and_lock_shared();
                return curNode;
            }
            auto children=curNode->children;
            curNode->divergeLock.unlock();
            return seekInternalNoInsert(str, children->getOrAddChild(str, curPos, curHeadId, curNode->stat), curPos);
        }

        auto newPos= checkPrefix(str.substr(curPos,std::string::npos),curNode->identity);
        auto newChild1=std::make_shared<Node>(curNode->identity.substr(newPos,std::string::npos),curNode->headID,std::move(curNode->stat),curNode->children);
        curNode->identity=curNode->identity.substr(0,newPos);
        curNode->headID=curHeadId++;
        curNode->stat=newChild1->stat->getNew();
        curNode->children.reset(new Children());
        curNode->children->addChild(newChild1);
        if(newPos!=str.length()){
            auto newChild2=std::make_shared<Node>(str.substr(newPos,std::string::npos), curHeadId++, curNode->stat->getNew());
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
        auto isPref=isPrefix(curNode->identity,str);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            curNode->divergeLock.unlock_shared();
            return seekInternalNoInsert(str, children->getChild(str, curPos), curPos);
        }
        return nullptr;
    }

    bool TierTree::isPrefix(const std::string &prefix, const std::string &full) {
        return prefix==full.substr(0, prefix.size());
    }

    int TierTree::checkPrefix(const std::string &a, const std::string &b) {
        for(auto i=0;i<min(a.length(),b.length());i++){
            if(a[i]!=b[i]){
                return i;
            }
        }
        return 0;
    }
}