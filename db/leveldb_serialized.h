#ifndef YCSB_C_LEVELDB_SERIALIZED_H
#define YCSB_C_LEVELDB_SERIALIZED_H

#include "core/db.h"

#include <leveldb/db.h>


namespace ycsbc
{

class LevelDBSerialized : public DB
{
    leveldb::DB *db_;

public:
    LevelDBSerialized(const std::string&);
    ~LevelDBSerialized();

    void Init() override;
    void Close() override;

    int Read(const std::string&, const std::string&, const std::vector<std::string>*, std::vector<KVPair>&);

    int Scan(const std::string&, const std::string&, int, const std::vector<std::string>*,std::vector<std::vector<KVPair>>&);

    int Update(const std::string&, const std::string&, std::vector<KVPair>&);

    int Insert(const std::string&, const std::string&, std::vector<KVPair>&);

    int Delete(const std::string&, const std::string&);
};

}

#endif