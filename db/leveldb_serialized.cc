#include "db/leveldb_serialized.h"
#include "db/utils/serialization.h"

#include <cassert>


namespace ycsbc
{

LevelDBSerialized::LevelDBSerialized(const std::string &path)
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db_);
    assert(status.ok());
}


LevelDBSerialized::~LevelDBSerialized()
{
    delete db_;
}


void LevelDBSerialized::Init()
{}


void LevelDBSerialized::Close()
{}


int LevelDBSerialized::Read(const std::string &table, const std::string &key, const std::vector<std::string> *fields, std::vector<KVPair> &result)
{
    std::string content;
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields(content);
    pushFields(map, fields, result);
    
    return kOK;
}


int LevelDBSerialized::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    leveldb::Iterator *it = db_->NewIterator(leveldb::ReadOptions());
    int iteration = 0;

    for (it->Seek(key); it->Valid() && iteration < record_count; it->Next(), ++iteration) 
    {
        std::vector<KVPair> value;
        FieldsMap map = deserializeFields(it->value().ToString());
        pushFields(map, fields, value);
        result.push_back(value);
    }

    delete it;
    return kOK;
}


int LevelDBSerialized::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    std::string content; 
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields(content);
    for (auto &pair : values)
        map.insert(pair);

    status = db_->Put(leveldb::WriteOptions(), key, serializeFields(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


int LevelDBSerialized::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    FieldsMap map;
    for (auto &pair : values)
        map.insert(pair);

    leveldb::Status status = db_->Put(leveldb::WriteOptions(), key, serializeFields(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


int LevelDBSerialized::Delete(const std::string &table, const std::string &key)
{
    leveldb::Status status = db_->Delete(leveldb::WriteOptions(), key);
    
    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

}