#ifndef SPTAGLIB_TIERTREE_H
#define SPTAGLIB_TIERTREE_H

#include <string>

namespace IVF{
    class TierTreeChildren{
        
    };
    class TierTreeNode{
    public:
        int headID;
        std::string identity;
        TierTreeChildren children;
    };
    class TierTree{
    public:
        TierTreeNode* root;
        int insert(std::string str);
        int curHeadId;
    };
}

#endif //SPTAGLIB_TIERTREE_H
