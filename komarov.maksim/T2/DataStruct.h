#ifndef MAX_DATASTRUCT_HPP
#define MAX_DATASTRUCT_HPP

#include <string>
#include <istream>
#include <ostream>

namespace max
{
    struct DataStruct
    {
        double              key1;
        unsigned long long  key2;
        std::string         key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dst);
    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
    bool cmpDataStruct(const DataStruct& a, const DataStruct& b);
}

#endif // MAX_DATASTRUCT_HPP