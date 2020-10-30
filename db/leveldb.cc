#include "db/leveldb.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>


using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


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
    std::string content;
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), encodeKey_(table, key), &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields_(content);
    pushFields_(map, fields, result);
    
    return kOK;
}

int LevelDB::Scan(const std::string &table, const std::string &key, int record_count, const std::vector<std::string> *fields,std::vector<std::vector<KVPair>> &result)
{
    leveldb::Iterator *it = db_->NewIterator(leveldb::ReadOptions());
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

int LevelDB::Update(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    std::string content, encKey = encodeKey_(table, key); 
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), encKey, &content);

    if (!status.ok())
        return kErrorNoData;

    FieldsMap map = deserializeFields_(content);
    for (auto &pair : values)
        map.insert(pair);

    status = db_->Put(leveldb::WriteOptions(), encKey, serializeFields_(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

int LevelDB::Insert(const std::string &table, const std::string &key, std::vector<KVPair> &values)
{
    FieldsMap map;
    for (auto &pair : values)
        map.insert(pair);

    leveldb::Status status = db_->Put(leveldb::WriteOptions(), encodeKey_(table, key), serializeFields_(map));

    if (!status.ok())
        return kErrorConflict;

    return kOK;
}

int LevelDB::Delete(const std::string &table, const std::string &key)
{
    leveldb::Status status = db_->Delete(leveldb::WriteOptions(), encodeKey_(table, key));
    
    if (!status.ok())
        return kErrorConflict;

    return kOK;
}


std::string LevelDB::serializeFields_(const FieldsMap &fields)
{
    ptree pt;
    for (auto &entry : fields)
        pt.put(entry.first, entry.second);
    
    std::ostringstream buf;
    write_json(buf, pt, false);

    return buf.str();
}

LevelDB::FieldsMap LevelDB::deserializeFields_(const std::string &str)
{
    ptree pt;
    std::istringstream buf(str);
    read_json(buf, pt);

    FieldsMap fields;
    for (auto &entry : pt)
        fields.insert(std::make_pair(entry.first, entry.second.data()));
        
    return fields;
}

std::string LevelDB::encodeKey_(const std::string &table, const std::string &key)
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

void LevelDB::pushFields_(const FieldsMap &map, const std::vector<std::string> *fields, std::vector<KVPair> &result)
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