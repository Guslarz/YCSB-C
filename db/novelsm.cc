#include "db/novelsm.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>


using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


namespace ycsbc
{

NoveLSM::NoveLSM(const std::string &dbdisk, const std::string &dbmem)
{
    novelsm::Options options;
    options.create_if_missing = true;

    novelsm::Status status = novelsm::DB::Open(options, dbdisk, dbmem, &db_);
    if (!status.ok())
        std::cerr << status.ToString() << std::endl;
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
    std::string content;
    novelsm::Status status = db_->Get(novelsm::ReadOptions(), encodeKey_(table, key), &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields_(content);
    pushFields_(map, fields, result);
    
    return kOK;
}

int NoveLSM::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    novelsm::Iterator *it = db_->NewIterator(novelsm::ReadOptions());
    int iteration = 0;

    for (it->Seek(encodeKey_(table, key)); it->Valid() && iteration < record_count; it->Next(), ++iteration) 
    {
        std::vector<KVPair> value;
        FieldsMap map = deserializeFields_(it->value().ToString());
        pushFields_(map, fields, value);
        result.push_back(value);
    }

    delete it;
    return kOK;
}

int NoveLSM::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    std::string content, encKey = encodeKey_(table, key); 
    novelsm::Status status = db_->Get(novelsm::ReadOptions(), encKey, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields_(content);
    for (auto &pair : values)
        map.insert(pair);

    status = db_->Put(novelsm::WriteOptions(), encKey, serializeFields_(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

int NoveLSM::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    FieldsMap map;
    for (auto &pair : values)
        map.insert(pair);

    novelsm::Status status = db_->Put(novelsm::WriteOptions(), encodeKey_(table, key), serializeFields_(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

int NoveLSM::Delete(const std::string &table, const std::string &key)
{
    novelsm::Status status = db_->Delete(novelsm::WriteOptions(), encodeKey_(table, key));
    
    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


std::string NoveLSM::serializeFields_(const FieldsMap &fields)
{
    ptree pt;
    for (auto &entry : fields)
        pt.put(entry.first, entry.second);
    
    std::ostringstream buf;
    write_json(buf, pt, false);

    return buf.str();
}

NoveLSM::FieldsMap NoveLSM::deserializeFields_(const std::string &str)
{
    ptree pt;
    std::istringstream buf(str);
    read_json(buf, pt);

    FieldsMap fields;
    for (auto &entry : pt)
        fields.insert(std::make_pair(entry.first, entry.second.data()));
        
    return fields;
}

std::string NoveLSM::encodeKey_(const std::string &table, const std::string &key)
{
    auto it = prefixes_.find(key);
    int prefix;
    if (it == prefixes_.end()) {
        prefix = maxPrefix_++;
        prefixes_.insert(std::make_pair(key, prefix));
    } else {
        prefix = it->second;
    }

    char bytes[4];
    for (int i = 0; i < 4; ++i)
        bytes[3 - i] = (prefix >> (8 * i)) & 0x8;

    return std::string(bytes) + key;
}

void NoveLSM::pushFields_(const FieldsMap &map, const std::vector<std::string> *fields, std::vector<KVPair> &result)
{
    if (fields) {
        for (auto &field : *fields)
            result.push_back(std::make_pair(field, map.at(field)));
    } else {
        for (auto &entry : map)
            result.push_back(entry);
    }
}

}