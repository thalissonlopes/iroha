

#include <string>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <memory>

// WIP
#include <iostream>

#include "entity.hpp"

namespace EntityRepository{

  std::shared_ptr<leveldb::DB> db;

  bool printStatus(leveldb::Status status){
    if(!status.ok()){
      // ToDo Logger
      std::cerr << status.ToString() << std::endl;
      return false;
    }
    return true;
  }

  void loadDb(){
    leveldb::Options options;
    options.create_if_missing = true;
    printStatus(leveldb::DB::Open(options, "/tmp/irohadb", &*db));
  }

  bool add(std::string uuid,Entity value){
    if(db == nullptr) loadDb();
    return printStatus(db->Put(leveldb::WriteOptions(), uuid, value));
  }

  bool remove(std::string uuid){
    if(db == nullptr) loadDb();
    return printStatus(db->Delete(leveldb::WriteOptions(), uuid));
  }

  bool update(std::string uuid,Entity value){
    if(db == nullptr) loadDb();
    Entity tmpValue;
    if(printStatus(db->Get(leveldb::ReadOptions(), uuid, &tmpValue))) {
      leveldb::WriteBatch batch;
      batch.Delete(uuid);
      batch.Put(uuid, value);
      return printStatus(db->Write(leveldb::WriteOptions(), &batch));
    }
    return false;
  }
  Entity find(std::string uuid){
    if(db == nullptr) loadDb();
    Entity value;
    printStatus(db->Get(leveldb::ReadOptions(), uuid, &value));
    return value;
  }
}
