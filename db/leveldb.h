#ifndef YCSB_C_LEVELDB_H
#define YCSB_C_LEVELDB_H

#include "core/db.h"

#include <leveldb/db.h>
#include <unordered_map>


namespace ycsbc
{

class LevelDB : public DB
{
    leveldb::DB *db_;
    std::unordered_map<std::string, unsigned int> prefixes_;
    unsigned int maxPrefix_ = 0;

    using FieldsMap = std::unordered_map<std::string, std::string>;

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
    static std::string serializeFields_(const FieldsMap&);
    static FieldsMap deserializeFields_(const std::string&);

    std::string encodeKey_(const std::string&, const std::string&);

    void pushFields_(const FieldsMap&, const std::vector<std::string>*, std::vector<KVPair>&);
};

}

#endif