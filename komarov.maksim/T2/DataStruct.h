#pragma once
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

    struct DelimiterIO               { char                exp; };
    struct DoubleLiteralIO           { double            & ref; };
    struct UnsignedLongLongLiteralIO { unsigned long long & ref; };
    struct StringIO                  { std::string       & ref; };
    struct LabelIO                   { std::string          exp; };

    class IOFmtGuard
    {
    public:
        explicit IOFmtGuard(std::basic_ios<char>& s);
        ~IOFmtGuard();
    private:
        std::basic_ios<char>& stream_;
        std::streamsize        width_;
        char                   fill_;
        std::streamsize        prec_;
        std::ios::fmtflags     flags_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest);
    std::istream& operator>>(std::istream& in, UnsignedLongLongLiteralIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);

    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}

#endif
