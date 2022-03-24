#include "inc/IVF/RocksDbKeyValueIO.h"
//
//namespace IVF {
//    bool RocksDbKeyValueIO::init(std::string path) {
//        rocksdb::Options options;
//        options.create_if_missing = true;
//        rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
//
//        //TODO Error
//        return (status == rocksdb::Status::OK());
//    }
//
//    std::istream *RocksDbKeyValueIO::get(const std::string &key) {
//        std::string dbValue;
//        std::cout << "get key:" << key << std::endl;
//        auto s = db->Get(rocksdb::ReadOptions(), key, &dbValue);
//        return new std::istringstream(dbValue);
//        //TODO Error
////    return (s == rocksdb::Status::OK());
//    }
//
//    bool RocksDbKeyValueIO::shutdown() {
//        rocksdb::Status s = db->Close();
//
//        delete db;
//        //TODO Error
//        return (s == rocksdb::Status::OK());
//    }
//
//    bool RocksDbKeyValueIO::set(const std::string &key, const std::string &value) {
//
//        std::cout << "set key:" << key << std::endl;
//        auto s = db->Put(rocksdb::WriteOptions(), key, value);
//
//        //TODO Error
//        return (s == rocksdb::Status::OK());
//    }
//}