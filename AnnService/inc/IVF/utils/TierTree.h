#ifndef SPTAGLIB_TIERTREE_H
#define SPTAGLIB_TIERTREE_H

#include <string>
#include <memory>
#include <utility>
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
            std::shared_ptr<Node> getOrAddChild(const std::string& str,int curPos);
            void addChild(std::shared_ptr<Node> child);
            //TODO lock per char
            std::shared_ptr<Node>* index;
        };
        class Node{
        public:
            Node(std::string ident,HeadIDType hid,void* stat,const std::shared_ptr<Children>& children=nullptr):identity(std::move(ident)){
                headID=hid;
                this->stat=stat;
                if(children==nullptr){
                    this->children=std::make_shared<Children>();
                }
                else{
                    this->children=children;
                }
            }
            HeadIDType headID;
            std::string identity;
            void* stat;
            std::shared_ptr<Children> children;
        };
    public:
        std::shared_ptr<Node> root;
        void init(void* statInitValue_in, void statAddOp_in(void*,void*), void statDelOp_in(void*,void*), void (void*));
        HeadIDType seek(const Term &term, void **rt_stat);
        HeadIDType set(const Term& term,void* stat);
        HeadIDType add(const Term& term,void* inputStat);
        HeadIDType del(const Term& term,void* inputStat);
        //TODO atomic
        int curHeadId;
    private:
        void* statInitValue;
        void (*statAddOp)(void*,void* inputStat);
        void (*statDelOp)(void*,void* inputStat);
        void (*deleteStat)(void*);
        std::shared_ptr<Node> seekInternalNoInsert(const std::string& str, std::shared_ptr<Node> curNode, int curPos);
        std::shared_ptr<Node> seekInternalWithInsert(const std::string& str,std::shared_ptr<Node> curNode,int curPos);
    };

    TierTree::Children::Children() {
        index=new std::shared_ptr<Node>[sizeof(char)];
        for(auto i=0;i<sizeof(char);i++){
            index[i]=nullptr;
        }
    }

    TierTree::Children::~Children() {
        delete[] index;
    }

    void TierTree::init(void *statInitValue_in, void (*statAddOp_in)(void *,void *), void (*statDelOp_in)(void *,void *), void(*deleteStat_in)(void*)) {
        statInitValue=statInitValue_in;
        statAddOp=statAddOp_in;
        statDelOp=statDelOp_in;
        deleteStat=deleteStat_in;
        curHeadId=0;
        root=std::make_shared<Node>("",curHeadId++,statInitValue);
    }


    HeadIDType TierTree::seek(const Term &term, void **rt_stat) {
        auto node = seekInternalNoInsert(term.getStr(),root,0);
        //TODO stat rlock
        if(rt_stat!=nullptr){
            *rt_stat=node->stat;
        }
        //release stat rlock
        auto hid=node->headID;
        //release diverge rlock
        return hid;
    }

    HeadIDType TierTree::set(const Term &term, void *stat) {
        auto node = seekInternalWithInsert(term.getStr(),root,0);
        //TODO stat wlock
        deleteStat(node->stat);
        node->stat=stat;
        //release stat wlock
        auto hid=node->headID;
        //release diverge rlock
        return hid;
    }

    HeadIDType TierTree::add(const Term &term, void *inputStat) {
        auto node = seekInternalWithInsert(term.getStr(),root,0);
        //TODO stat wlock
        (*statAddOp)(node->stat, inputStat);
        //release stat wlock
        auto hid=node->headID;
        //release diverge rlock
        return hid;
    }

    HeadIDType TierTree::del(const Term &term, void *inputStat) {
        auto node = seekInternalNoInsert(term.getStr(),root,0);
        //TODO stat wlock
        (*statDelOp)(node->stat, inputStat);
        //release stat wlock
        auto hid=node->headID;
        //release diverge rlock
        return hid;
    }

    std::shared_ptr<TierTree::Node> TierTree::seekInternalWithInsert(const std::string &str, std::shared_ptr<TierTree::Node> curNode, int curPos) {
        if(curNode==nullptr)return nullptr;
        //TODO diverge rlock
        auto isPref=isPrefix(curNode->identity,str);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            //release diverge rlock
            return seekInternalNoInsert(str, children->getOrAddChild(str, curPos), curPos);
        }
        else{
            //TODO diverge wlock
            auto newPos= checkPrefix(str.substr(curPos,std::string::npos),curNode->identity);
            auto newChild1=std::make_shared<Node>(curNode->identity.substr(newPos,std::string::npos),curHeadId++,curNode->stat,curNode->children);
            curNode->identity=curNode->identity.substr(0,newPos);
            curNode->stat=statInitValue;
            curNode->children.reset(new Children());
            curNode->children->addChild(newChild1);
            if(newPos==str.length()){
                //TODO diverge rlock
                //down grade to rlock?
                return curNode;
            }
            else{
                auto newChild2=std::make_shared<Node>(str.substr(newPos,std::string::npos),curHeadId++,statInitValue);
                //TODO diverge rlock
                // for newChild2
                curNode->children->addChild(newChild2);
                //release diverge wlock
                return newChild2;
            }
        }
        return nullptr;
    }


    std::shared_ptr<TierTree::Node> TierTree::seekInternalNoInsert(const std::string &str, std::shared_ptr<TierTree::Node> curNode, int curPos) {
        if(curNode==nullptr)return nullptr;
        //TODO diverge rlock
        auto isPref=isPrefix(curNode->identity,str);
        if(isPref){
            auto identityLen=curNode->identity.length();
            curPos+=identityLen;
            if(curPos==str.length()){
                return curNode;
            }
            auto children=curNode->children;
            //release diverge rlock
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
