#include "DataStruct.h"

#include <regex>
#include <sstream>
#include <iomanip>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char temp = '0';
    in >> temp;
    if (in && (temp != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, KeyNumberIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    in >> DelimiterIO{ 'k' } >> DelimiterIO{ 'e' } >> DelimiterIO{ 'y' } >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string numStr;
    std::getline(in, numStr, ':');
    in.putback(':');

    const std::regex sciPattern(R"(^[+-]?(\d+\.?\d*|\.\d+)[eE][+-]?\d+$)");
    if (!std::regex_match(numStr, sciPattern)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(numStr);
    iss >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, CharIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return in >> DelimiterIO{ '\'' } >> dest.ref >> DelimiterIO{ '\'' };
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    DataStruct input;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    int keyName = 0;
    for (int i = 0; (i < 3) && in; ++i) {
        in >> KeyNumberIO{ keyName };
        if (keyName == 1) {
            in >> DoubleSciIO{ input.key1 };
        }
        else if (keyName == 2) {
            in >> CharIO{ input.key2 };
        }
        else if (keyName == 3) {
            in >> StringIO{ input.key3 };
        }
        else {
            in.setstate(std::ios::failbit);
        }
        in >> DelimiterIO{ ':' };
    }
    in >> DelimiterIO{ ')' };
    if (in) {
        dest = input;
    }
    return in;
}

bool compareDS(const DataStruct& first, const DataStruct& second) {
    if (first.key1 != second.key1) {
        return first.key1 < second.key1;
    }
    else if (first.key2 != second.key2) {
        return first.key2 < second.key2;
    }
    else {
        return first.key3.length() < second.key3.length();
    }
}

std::string formatExponent(const std::string& numStr) {
    std::regex exp_regex(R"(e([+-]?)(0*)([1-9]\d*))");
    return std::regex_replace(numStr, exp_regex, "e$1$3");
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard fmtguard(out);

    std::ostringstream oss;
    oss << std::scientific << std::setprecision(1) << src.key1;
    std::string numStr = oss.str();

    out << "(:key1 " << formatExponent(numStr);
    out << ":key2 '" << src.key2;
    out << "':key3 \"" << src.key3 << "\":)";
    return out;
}
