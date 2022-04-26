#ifndef SPTAGLIB_TERMINDEX_H
#define SPTAGLIB_TERMINDEX_H

#include "inc/IVF/interfaces/KeywordIndex.h"
#include "inc/IVF/interfaces/KeyValueIO.h"
#include "inc/IVF/interfaces/StringHeadIndex.h"
#include "inc/IVF/utils/TermSetDataHolder.h"

namespace IVF {

    class TermIndex : public KeywordIndex {
    public:
        bool updateAllFinished() override;
        void close() override;
        HeadIDType getHeadID(const std::string &str, std::string *rt_stat);
        void getPostingList(HeadIDType headID,std::istream **value);
        void setPostingList(HeadIDType headID,  const std::string &value);
        void buildIndex(std::unique_ptr<TermSetDataHolder> dataHolder, int threadNum, const std::string& headIndexFile);
        void setHeadIndex(std::unique_ptr<StringHeadIndex> head_index);
        void setKV(std::unique_ptr<KeyValueIO> kvio);
        void setScoreScheme(std::unique_ptr<ScoreScheme> scoreScheme);
        void loadHeadIndex(const std::string& path);
        void loadHeadIndexWarmup(const std::string& path);
        std::unique_ptr<ScoreScheme> getCollectionStatHolder();
    private:
        std::unique_ptr<KeyValueIO> db;
        std::unique_ptr<StringHeadIndex> head_index;
        std::unique_ptr<CollectionStatistic> collectionStatistic;
        std::unique_ptr<ScoreScheme> scoreScheme;
    };


}

#endif //SPTAGLIB_TERMINDEX_H
