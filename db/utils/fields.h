#ifndef YCSB_C_FIELDS_H
#define YCSB_C_FIELDS_H

#include <vector>
#include <string>


namespace ycsbc
{

class FieldSuffixGenerator
{
    int counter_ = 0;

    const static std::string fieldSuffix_;

public:
    FieldSuffixGenerator() = default;

    std::string operator()();
};

std::vector<std::string> generateFieldSuffixes(int);


};


#endif