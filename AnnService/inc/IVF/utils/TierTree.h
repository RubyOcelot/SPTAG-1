#ifndef SPTAGLIB_TIERTREE_H
#define SPTAGLIB_TIERTREE_H

#include <string>
#include <memory>
#include <utility>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <boost/thread/shared_mutex.hpp>
#include "inc/IVF/Term.h"
#include "inc/IVF/ForwardDefine.h"
#include "inc/Core/Common.h"


namespace IVF{
    class TierTree{
        static bool isPrefix(const std::string& prefix,const std::string& full);
        static int checkPrefix(const std::string& a,const std::string& b);
        class Node;
        class Children{
        public:
            Children();
            ~Children();
            std::shared_ptr<Node> getChild(const std::string& str,int curPos);
            std::shared_ptr<Node> getOrAddChild(const std::string& str,int curPos,std::atomic<HeadIDType>& curHeadId_ref,void* (*statInitValue)());
            void addChild(const std::shared_ptr<Node>& child);
            std::shared_ptr<Node>* charIndex;
            std::shared_mutex* charRWLock;
        };
        class Node{
        public:
            Node(std::string ident, HeadIDType hid, void* (*statInit)(),void (*delStat)(void*), const std::shared_ptr<Children>& children=nullptr): identity(std::move(ident)){
                headID=hid;
                this->stat=statInit();
                if(children==nullptr){
                    this->children=std::make_shared<Children>();
                }
                else{
                    this->children=children;
                }
                this->deleteStat=delStat;
            }
            ~Node(){
                (*deleteStat)(stat);
            }
            HeadIDType headID;
            std::string identity;
            void* stat;
            std::shared_ptr<Children> children;
            void (*deleteStat)(void*);
            mutable boost::shared_mutex divergeLock;
        };
    public:
        std::shared_ptr<Node> root;
        void init(void* (*statInitValue_in)(), void (*statAddOp_in)(void*,void*), void (*statDelOp_in)(void*,void*), void (*deleteStat_in)(void*),void* (*outputStat_in)(void*));
        HeadIDType seek(const Term &term, void **rt_stat);
        HeadIDType set(const Term& term,void* stat);
        HeadIDType add(const Term& term,void* inputStat);
        HeadIDType del(const Term& term,void* inputStat);
        std::atomic<HeadIDType> curHeadId;
    private:
        void* (*initStat)();
        void (*statOpForAdd)(void*, void* inputStat);
        void (*statOpForDel)(void*, void* inputStat);
        void (*deleteStat)(void*);

        //copy non-atomic value for stat
        void* (*outputStat)(void*);
        std::shared_ptr<Node> seekInternalNoInsert(const std::string& str, std::shared_ptr<Node> curNode, int curPos);
        std::shared_ptr<Node> seekInternalWithInsert(const std::string& str,std::shared_ptr<Node> curNode,int curPos);
    };

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

    std::shared_ptr<TierTree::Node> TierTree::Children::getChild(const std::string &str, int curPos) {
        unsigned char fc=str[curPos];
        charRWLock[fc].lock_shared();
        auto node=charIndex[fc];
        charRWLock[fc].unlock_shared();
        return node;
    }

    std::shared_ptr<TierTree::Node> TierTree::Children::getOrAddChild(const std::string &str, int curPos, std::atomic<HeadIDType>& curHeadId_ref,void* (*statInitValue)()) {
        unsigned char fc=str[curPos];
        charRWLock[fc].lock_shared();
        auto node=charIndex[fc];
        charRWLock[fc].unlock_shared();
        if(node!=nullptr)
            return node;

        charRWLock[fc].lock();
        node=charIndex[fc];
        if(node==nullptr) {
            node=std::make_shared<Node>(str.substr(curPos,std::string::npos), curHeadId_ref++, statInitValue);
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

    void TierTree::init(void* (*statInitValue_in)(), void (*statAddOp_in)(void *,void *), void (*statDelOp_in)(void *,void *), void(*deleteStat_in)(void*),void* (*outputStat_in)(void*)) {
        initStat=statInitValue_in;
        statOpForAdd=statAddOp_in;
        statOpForDel=statDelOp_in;
        deleteStat=deleteStat_in;
        outputStat=outputStat_in;
        curHeadId=0;
        root=std::make_shared<Node>("", curHeadId++, initStat);
    }


    HeadIDType TierTree::seek(const Term &term, void **rt_stat) {
        auto node = seekInternalNoInsert(term.getStr(),root,0);
        if(rt_stat!=nullptr){
            *rt_stat=(*outputStat)(node->stat);
        }
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::set(const Term &term, void *stat) {
        auto node = seekInternalWithInsert(term.getStr(),root,0);
        deleteStat(node->stat);
        node->stat=stat;
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::add(const Term &term, void *inputStat) {
        auto node = seekInternalWithInsert(term.getStr(),root,0);
        (*statOpForAdd)(node->stat, inputStat);
        auto hid=node->headID;
        node->divergeLock.unlock_shared();
        return hid;
    }

    HeadIDType TierTree::del(const Term &term, void *inputStat) {
        auto node = seekInternalNoInsert(term.getStr(),root,0);
        (*statOpForDel)(node->stat, inputStat);
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
            return seekInternalNoInsert(str, children->getOrAddChild(str, curPos, curHeadId, initStat), curPos);
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
            return seekInternalNoInsert(str, children->getOrAddChild(str, curPos, curHeadId, initStat), curPos);
        }

        auto newPos= checkPrefix(str.substr(curPos,std::string::npos),curNode->identity);
        auto newChild1=std::make_shared<Node>(curNode->identity.substr(newPos,std::string::npos),curHeadId++,curNode->stat,curNode->children);
        curNode->identity=curNode->identity.substr(0,newPos);
        curNode->stat=initStat();
        curNode->children.reset(new Children());
        curNode->children->addChild(newChild1);
        if(newPos!=str.length()){
            auto newChild2=std::make_shared<Node>(str.substr(newPos,std::string::npos), curHeadId++, initStat);
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

#endif //SPTAGLIB_TIERTREE_H
