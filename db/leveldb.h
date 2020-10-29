#ifndef YCSB_C_LEVELDB_H
#define YCSB_C_LEVELDB_H

#include "core/db.h"

#include "leveldb/db.h"


namespace ycsbc
{

class LevelDB : public DB
{
    leveldb::DB *db_;

public:
    LevelDB(const std::string &filename);
    ~LevelDB();

    void Init() override;
    void Close() override;

    int Read(const std::string &table, const std::string &key, const std::vector<std::string> *fields, std::vector<KVPair> &result);

    int Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result);

    int Update(const std::string &table, const std::string &key, std::vector<KVPair> &values);

    int Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values);

    int Delete(const std::string &table, const std::string &key);
};

}

#endif