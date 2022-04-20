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
#include "inc/IVF/CommonUse.h"
#include "inc/Core/Common.h"
#include "inc/IVF/interfaces/KeywordStatistic.h"
#include "inc/IVF/interfaces/StringHeadIndex.h"


namespace IVF{
    class TierTree: public StringHeadIndex{
        static bool isPrefix(const std::string& prefix,const std::string& full);
        static int checkPrefix(const std::string& a,const std::string& b);
        class Node;
        class Children{
        public:
            Children();
            ~Children();
            std::shared_ptr<Node> getChild(const std::string& str,int curPos) const;
            std::shared_ptr<Node> getOrAddChild(const std::string& str, int curPos, std::atomic<HeadIDType>& curHeadId_ref,
                                                const std::unique_ptr<KeywordStatistic> &stat);
            void addChild(const std::shared_ptr<Node>& child);
            std::shared_ptr<Node>* charIndex;
            std::shared_mutex* charRWLock;
        };
        class Node{
        public:
            Node(std::string ident, HeadIDType hid, std::unique_ptr<KeywordStatistic> stat, const std::shared_ptr<Children>& children=nullptr): identity(std::move(ident)),headID(hid),stat(std::move(stat)){
                if(this->stat == nullptr){
                    //TODO error
                }
                if(children==nullptr){
                    this->children=std::make_shared<Children>();
                }
                else{
                    this->children=children;
                }
            }
            HeadIDType headID;
            std::string identity;
            std::unique_ptr<KeywordStatistic> stat;
            std::shared_ptr<Children> children;
            mutable boost::shared_mutex divergeLock;
        };
    public:
        explicit TierTree(std::unique_ptr<KeywordStatistic> emptyStat);
        std::shared_ptr<Node> root;
        HeadIDType seek(const std::string &str, std::string *rt_stat) override;
        HeadIDType set(const std::string &str, std::string stat) override;
        HeadIDType add(const std::string &str, std::string inputStat) override;
        HeadIDType del(const std::string &str, std::string inputStat) override;
        std::atomic<HeadIDType> curHeadId;
    private:
        std::shared_ptr<Node> seekInternalNoInsert(const std::string& str, std::shared_ptr<Node> curNode, int curPos);
        std::shared_ptr<Node> seekInternalWithInsert(const std::string& str,std::shared_ptr<Node> curNode,int curPos);
    };


}

#endif //SPTAGLIB_TIERTREE_H
