#ifndef SPTAGLIB_KEYVALUEIO_H
#define SPTAGLIB_KEYVALUEIO_H

#include "inc/Core/Common.h"
#include "inc/IVF/CommonUse.h"


namespace IVF {
    class KeyValueIO {
    public:
        virtual ~KeyValueIO()=0;

        virtual bool Initialize(const char *filePath) = 0;

        virtual void ShutDown() = 0;

        virtual SPTAG::ErrorCode Get(const std::string &key, std::istringstream *value) = 0;

        virtual SPTAG::ErrorCode Get(HeadIDType key, std::istringstream *value) = 0;

        virtual SPTAG::ErrorCode Put(const std::string &key, const std::string &value) = 0;

        virtual SPTAG::ErrorCode Put(HeadIDType key, const std::string &value) = 0;

        virtual SPTAG::ErrorCode Merge(HeadIDType key, const std::string &value) = 0;

        virtual SPTAG::ErrorCode Delete(HeadIDType key) = 0;

        virtual void ForceCompaction()=0;
    };
}

#endif //SPTAGLIB_KEYVALUEIO_H
