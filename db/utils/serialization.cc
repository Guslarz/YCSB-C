#include "db/utils/serialization.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>


using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


namespace ycsbc
{

std::string serializeFields(const FieldsMap &fields)
{
    ptree pt;
    for (auto &entry : fields)
        pt.put(entry.first, entry.second);
    
    std::ostringstream buf;
    write_json(buf, pt, false);

    return buf.str();
}


FieldsMap deserializeFields(const std::string &str)
{
    ptree pt;
    std::istringstream buf(str);
    read_json(buf, pt);

    FieldsMap fields;
    for (auto &entry : pt)
        fields.insert(std::make_pair(entry.first, entry.second.data()));
        
    return fields;
}


void pushFields(const FieldsMap &map, const std::vector<std::string> *fields, std::vector<DB::KVPair> &result)
{
    if (fields) {
        for (auto &field : *fields)
            result.push_back(std::make_pair(field, map.at(field)));
    } else {
        for (auto &entry : map)
            result.push_back(entry);
    }
}

};
