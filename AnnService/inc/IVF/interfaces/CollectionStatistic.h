#ifndef SPTAGLIB_COLLECTIONSTATISTIC_H
#define SPTAGLIB_COLLECTIONSTATISTIC_H

#include <string>
#include <memory>

namespace IVF{

class CollectionStatistic {
public:
    virtual void reset()=0;
    virtual void set(const std::string &)=0;
    virtual void modForAdd(const std::string &)=0;
    virtual void modForDel(const std::string &)=0;
    virtual std::string getContent()=0;
    virtual std::unique_ptr<CollectionStatistic> clone()=0;
};
}

#endif //SPTAGLIB_COLLECTIONSTATISTIC_H
