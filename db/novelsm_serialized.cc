#include "db/novelsm_serialized.h"
#include "db/utils/serialization.h"

#include <cassert>


namespace ycsbc
{

NoveLSMSerialized::NoveLSMSerialized(const std::string &dbdisk, const std::string &dbmem)
{
    novelsm::Options options;
    options.create_if_missing = true;

    novelsm::Status status = novelsm::DB::Open(options, dbdisk, dbmem, &db_);
    assert(status.ok());
}


NoveLSMSerialized::~NoveLSMSerialized()
{
    delete db_;
}


void NoveLSMSerialized::Init()
{}


void NoveLSMSerialized::Close()
{}


int NoveLSMSerialized::Read(const std::string &table, const std::string &key, const std::vector<std::string> *fields, std::vector<KVPair> &result)
{
    std::string content;
    novelsm::Status status = db_->Get(novelsm::ReadOptions(), key, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields(content);
    pushFields(map, fields, result);
    
    return kOK;
}


int NoveLSMSerialized::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    novelsm::Iterator *it = db_->NewIterator(novelsm::ReadOptions());
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


int NoveLSMSerialized::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    std::string content; 
    novelsm::Status status = db_->Get(novelsm::ReadOptions(), key, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields(content);
    for (auto &pair : values)
        map.insert(pair);

    status = db_->Put(novelsm::WriteOptions(), key, serializeFields(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


int NoveLSMSerialized::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    FieldsMap map;
    for (auto &pair : values)
        map.insert(pair);

    novelsm::Status status = db_->Put(novelsm::WriteOptions(), key, serializeFields(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


int NoveLSMSerialized::Delete(const std::string &table, const std::string &key)
{
    novelsm::Status status = db_->Delete(novelsm::WriteOptions(), key);
    
    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

}