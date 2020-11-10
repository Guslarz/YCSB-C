#include "db/leveldb.h"
#include "db/utils/fields.h"

#include <leveldb/write_batch.h>

#include <iostream>
#include <cassert>


namespace ycsbc
{

const int LevelDB::fieldCount_ = 10;
const std::vector<std::string> LevelDB::fieldSuffixes_ = generateFieldSuffixes(LevelDB::fieldCount_);


LevelDB::LevelDB(const std::string &path)
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db_);
    assert(status.ok());
}


LevelDB::~LevelDB()
{
    delete db_;
}


void LevelDB::Init()
{}


void LevelDB::Close()
{}


int LevelDB::Read(const std::string &table, const std::string &key, const std::vector<std::string> *fields, std::vector<KVPair> &result)
{
    leveldb::Iterator *it = db_->NewIterator(leveldb::ReadOptions());
    it->Seek(key + fieldSuffixes_[0]);
    if (!it->Valid())
        return kErrorNoData;

    //  No workload uses fields in read
    assert(fields == nullptr);
    readAllFields_(it, result);

    assert(it->status().ok());
    delete it;

    return kOK;
}


int LevelDB::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    leveldb::Iterator *it = db_->NewIterator(leveldb::ReadOptions());
    it->Seek(key + fieldSuffixes_[0]);
    if (!it->Valid())
        return kErrorNoData;

    //  Same as read
    assert(fields == nullptr);
    for (int i = 0; i < record_count && it->Valid();)
    {
        std::vector<KVPair> currResult;
        readAllFields_(it, currResult);
        result.push_back(currResult);
    }

    assert(it->status().ok());
    delete it;

    return kOK;
}


int LevelDB::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    return Insert(table, key, values);
}


int LevelDB::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    leveldb::WriteBatch batch;
    for (const auto &pair : values)
    {
        batch.Put(key + pair.first, pair.second);
    }

    leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);

    assert(status.ok());

    return kOK;
}


int LevelDB::Delete(const std::string &table, const std::string &key)
{
    leveldb::WriteBatch batch;
    for (int i = 0; i < fieldCount_; ++i)
    {
        batch.Delete(key + fieldSuffixes_[i]);
    }

    leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);

    assert(status.ok());

    return kOK;
}


int LevelDB::readAllFields_(leveldb::Iterator *const it, std::vector<KVPair> &result)
{
    for (int i = 0; i < fieldCount_ && it->Valid(); ++i)
    {
        result.push_back(std::make_pair(it->key().ToString() + fieldSuffixes_[i], it->value().ToString()));
        it->Next();
    }
    return kOK;
}

}