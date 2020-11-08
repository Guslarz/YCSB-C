#ifndef YCSB_C_LEVELDB_H
#define YCSB_C_LEVELDB_H

#include "core/db.h"

#include <leveldb/db.h>
#include <unordered_set>


namespace ycsbc
{

class LevelDB : public DB
{
    leveldb::DB *db_;

    static const int fieldCount_;
    static const std::vector<std::string> fieldSuffixes_;

public:
    LevelDB(const std::string&);
    ~LevelDB();

    void Init() override;
    void Close() override;

    int Read(const std::string&, const std::string&, const std::vector<std::string>*, std::vector<KVPair>&);

    int Scan(const std::string&, const std::string&, int, const std::vector<std::string>*,std::vector<std::vector<KVPair>>&);

    int Update(const std::string&, const std::string&, std::vector<KVPair>&);

    int Insert(const std::string&, const std::string&, std::vector<KVPair>&);

    int Delete(const std::string&, const std::string&);

private:
    static int readAllFields_(leveldb::Iterator *const, std::vector<KVPair>&);
};

}

#endif