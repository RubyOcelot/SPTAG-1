#ifndef SPTAGLIB_ROCKSDBIO_H
#define SPTAGLIB_ROCKSDBIO_H

#include <string>
#include "inc/Core/Common.h"

#include "inc/Helper/KeyValueIO.h"
#include "inc/Helper/StringConvert.h"
#include "inc/IVF/interfaces/KeyValueIO.h"

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "rocksdb/merge_operator.h"

using namespace SPTAG;

namespace IVF {

    class RocksDBIO: public KeyValueIO{
    public:
        RocksDBIO() = default;

        ~RocksDBIO() override  {
            db->Close();
            DestroyDB(dbPath, dbOptions);
            delete db;
        }

        bool Initialize(const char *filePath) override  {
            dbPath = std::string(filePath);
            dbOptions.create_if_missing = true;
            dbOptions.IncreaseParallelism();
            dbOptions.OptimizeLevelStyleCompaction();
            dbOptions.merge_operator.reset(new AppendMergeOperator);

            auto s = rocksdb::DB::Open(dbOptions, dbPath, &db);
            LOG(Helper::LogLevel::LL_Info, "Inverted Index: New Rocksdb: %s\n", filePath);
            return s == rocksdb::Status::OK();
        }

        void ShutDown() override  {
            db->Close();
            DestroyDB(dbPath, dbOptions);
            delete db;
        }

        ErrorCode Get(const std::string &key, std::istream **value) override  {
            if(value==nullptr)return ErrorCode::Fail;
            std::string strValue;
            auto s = db->Get(rocksdb::ReadOptions(), key, &strValue);
            auto stream=new std::istringstream(strValue);
            *value=stream;
            if (s == rocksdb::Status::OK()) {
                return ErrorCode::Success;
            } else {
                return ErrorCode::Fail;
            }
        }

        ErrorCode Get(SizeType key, std::istream **value) override  {
            return Get(Helper::Convert::Serialize<SizeType>(&key), value);
        }

        ErrorCode Put(const std::string &key, const std::string &value) override {
            auto s = db->Put(rocksdb::WriteOptions(), key, value);
            if (s == rocksdb::Status::OK()) {
                return ErrorCode::Success;
            } else {
                return ErrorCode::Fail;
            }
        }

        ErrorCode Put(SizeType key, const std::string &value) override  {
            return Put(Helper::Convert::Serialize<SizeType>(&key), value);
        }

        class AppendMergeOperator : public rocksdb::AssociativeMergeOperator {
        public:
            bool Merge(const rocksdb::Slice &key, const rocksdb::Slice *existing_value,
                       const rocksdb::Slice &value, std::string *new_value,
                       rocksdb::Logger *logger) const override {
                std::string newPosting;
                if (existing_value) {
                    newPosting += (*existing_value).ToString();
                    newPosting += value.ToString();
                } else {
                    newPosting += value.ToString();
                }
                *new_value = newPosting;
                return true;
            }

            const char *Name() const override {
                return "AppendMergeOperator";
            }
        };

        ErrorCode Merge(SizeType key, const std::string &value) override {
            if (value.empty()) {
                LOG(Helper::LogLevel::LL_Error, "Error! empty append posting!\n");
            }
            auto s = db->Merge(rocksdb::WriteOptions(), Helper::Convert::Serialize<int>(&key, 1), value);
            if (s == rocksdb::Status::OK()) {
                return ErrorCode::Success;
            } else {
                return ErrorCode::Fail;
            }
        }

        ErrorCode Delete(SizeType key) override  {
            auto s = db->Delete(rocksdb::WriteOptions(), Helper::Convert::Serialize<int>(&key, 1));
            if (s == rocksdb::Status::OK()) {
                return ErrorCode::Success;
            } else {
                return ErrorCode::Fail;
            }
        }

        void ForceCompaction() override {
            LOG(Helper::LogLevel::LL_Info, "Start Compaction\n");
            db->CompactRange(rocksdb::CompactRangeOptions(), nullptr, nullptr);
            LOG(Helper::LogLevel::LL_Info, "Finish Compaction\n");
        }

    private:
        std::string dbPath;
        rocksdb::DB *db{};
        rocksdb::Options dbOptions;
    };
}

#endif //SPTAGLIB_ROCKSDBIO_H
