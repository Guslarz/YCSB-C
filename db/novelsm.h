#ifndef YCSB_C_NOVELSM_H
#define YCSB_C_NOVELSM_H

#include "core/db.h"

#include <novelsm/db.h>
#include <unordered_set>


namespace ycsbc
{

class NoveLSM : public DB
{
    novelsm::DB *db_;

    static const int fieldCount_;
    static const std::vector<std::string> fieldSuffixes_;

public:
    NoveLSM(const std::string&, const std::string&);
    ~NoveLSM();

    void Init() override;
    void Close() override;

    int Read(const std::string&, const std::string&, const std::vector<std::string>*, std::vector<KVPair>&);

    int Scan(const std::string&, const std::string&, int, const std::vector<std::string>*,std::vector<std::vector<KVPair>>&);

    int Update(const std::string&, const std::string&, std::vector<KVPair>&);

    int Insert(const std::string&, const std::string&, std::vector<KVPair>&);

    int Delete(const std::string&, const std::string&);

private:
    static int readAllFields_(novelsm::Iterator *const, std::vector<KVPair>&);
};

}

#endif