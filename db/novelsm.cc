#include "db/novelsm.h"
#include "db/utils/fields.h"

#include <novelsm/write_batch.h>

#include <iostream>
#include <cassert>


namespace ycsbc
{

const int NoveLSM::fieldCount_ = 10;
const std::vector<std::string> NoveLSM::fieldSuffixes_ = generateFieldSuffixes(NoveLSM::fieldCount_);


NoveLSM::NoveLSM(const std::string &dbdisk, const std::string &dbmem)
{
    novelsm::Options options;
    options.create_if_missing = true;

    novelsm::Status status = novelsm::DB::Open(options, dbdisk, dbmem, &db_);
    assert(status.ok());
}


NoveLSM::~NoveLSM()
{
    delete db_;
}


void NoveLSM::Init()
{}


void NoveLSM::Close()
{}


int NoveLSM::Read(const std::string &table, const std::string &key, const std::vector<std::string> *fields, std::vector<KVPair> &result)
{
    novelsm::Iterator *it = db_->NewIterator(novelsm::ReadOptions());
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


int NoveLSM::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    novelsm::Iterator *it = db_->NewIterator(novelsm::ReadOptions());
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


int NoveLSM::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    return Insert(table, key, values);
}


int NoveLSM::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    novelsm::WriteBatch batch;
    for (const auto &pair : values)
    {
        batch.Put(key + pair.first, pair.second);
    }

    novelsm::Status status = db_->Write(novelsm::WriteOptions(), &batch);

    assert(status.ok());

    return kOK;
}


int NoveLSM::Delete(const std::string &table, const std::string &key)
{
    novelsm::WriteBatch batch;
    for (int i = 0; i < fieldCount_; ++i)
    {
        batch.Delete(key + fieldSuffixes_[i]);
    }

    novelsm::Status status = db_->Write(novelsm::WriteOptions(), &batch);

    assert(status.ok());

    return kOK;
}


int NoveLSM::readAllFields_(novelsm::Iterator *const it, std::vector<KVPair> &result)
{
    for (int i = 0; i < fieldCount_ && it->Valid(); ++i)
    {
        result.push_back(std::make_pair(it->key().ToString() + fieldSuffixes_[i], it->value().ToString()));
        it->Next();
    }
    return kOK;
}

}