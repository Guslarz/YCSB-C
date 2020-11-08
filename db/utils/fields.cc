#include "db/utils/fields.h"

#include <algorithm>


namespace ycsbc
{

const std::string FieldSuffixGenerator::fieldSuffix_ = "field";

std::string FieldSuffixGenerator::operator()()
{
    return fieldSuffix_ + std::to_string(counter_++);
}

std::vector<std::string> generateFieldSuffixes(int count)
{
    std::vector<std::string> vec(count);
    std::generate(vec.begin(), vec.end(), FieldSuffixGenerator());
    return vec;
}

};