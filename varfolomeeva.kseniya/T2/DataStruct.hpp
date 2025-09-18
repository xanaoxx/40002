#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <complex>
#include <utility>
#include <cctype>
#include <iomanip>
#include <iterator>

namespace dStruct
{
    struct DataStruct
    {
        double key1;
        long long key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct SllLit
    {
        long long& value;
    };

    struct DBLLit
    {
        double& value;
    };

    struct StrKey
    {
        std::string& str;
    };

    struct StringIO
    {
        const char* expected;
    };


    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
    };
}

#endif
