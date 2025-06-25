#ifndef DATA_STRUCT
#define DATA_STRUCT

#include <string>
#include <iostream>

class iofmtguard
{
public:
    iofmtguard(std::basic_ios< char >& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {
    }
    ~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
};

struct DelimiterIO
{
    char exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

struct KeyNumberIO
{
    int& ref;
};

std::istream& operator>>(std::istream& in, KeyNumberIO&& dest);

struct StringIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, StringIO&& dest);

struct DoubleSciIO
{
    double& ref;
};

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);

struct CharIO
{
    char& ref;
};

std::istream& operator>>(std::istream& in, CharIO&& dest);

struct DataStruct
{
    double key1;
    char key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& dest);

bool compareDS(const DataStruct& first, const DataStruct& second);

std::ostream& operator<<(std::ostream& out, const DataStruct& src);

#endif
