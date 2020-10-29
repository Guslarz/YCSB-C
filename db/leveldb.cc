#include "db/leveldb.h"

#include <iostream>


namespace ycsbc
{

LevelDB::LevelDB(const std::string &path)
{
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db_);
    if (!status.ok())
        std::cerr << status.ToString() << std::endl;
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
    return kOK;
}

int LevelDB::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    return kOK;
}

int LevelDB::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    return kOK;
}

int LevelDB::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    return kOK;
}

int LevelDB::Delete(const std::string &table, const std::string &key)
{
    return kOK;
}

}