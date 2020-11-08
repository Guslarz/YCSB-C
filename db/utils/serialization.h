#ifndef YCSB_C_SERIALIZATION_H
#define YCSB_C_SERIALIZATION_H

#include <unordered_map>
#include <vector>

#include "core/db.h"

namespace ycsbc
{

using FieldsMap = std::unordered_map<std::string, std::string>;

std::string serializeFields(const FieldsMap&);
FieldsMap deserializeFields(const std::string&);    
void pushFields(const FieldsMap&, const std::vector<std::string>*, std::vector<DB::KVPair>&);

};

#endif