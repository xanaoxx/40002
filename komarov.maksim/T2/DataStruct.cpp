#include "DataStruct.h"
#include <sstream>
#include <iomanip>
#include <vector>

namespace max {

class IOfmtGuard {
    std::ios&           s_;
    std::ios::fmtflags  f_;
    std::streamsize     p_, w_;
    char                fill_;
public:
    IOfmtGuard(std::ios& s)
     : s_(s)
     , f_(s.flags())
     , p_(s.precision())
     , w_(s.width())
     , fill_(s.fill())
    {}
    ~IOfmtGuard() {
        s_.flags(f_);
        s_.precision(p_);
        s_.width(w_);
        s_.fill(fill_);
    }
};

static bool parseLine(const std::string& line, DataStruct& tmp) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string chunk;
    while (std::getline(ss, chunk, ':'))
        if (!chunk.empty())
            parts.push_back(chunk);
    if (parts.size() != 5 || parts[0] != "(" || parts[4] != ")")
        return false;
    try {
        for (int i = 1; i <= 3; ++i) {
            std::istringstream ps(parts[i]);
            std::string key; ps >> key;
            if (key == "key1") {
                std::string val; ps >> val;
                if (val.back()!='d' && val.back()!='D') return false;
                tmp.key1 = std::stod(val.substr(0, val.size()-1));
            }
            else if (key == "key2") {
                std::string val; ps >> val;
                if (val.size()<4 ||
                   (val.substr(val.size()-3)!="ull" &&
                    val.substr(val.size()-3)!="ULL"))
                    return false;
                tmp.key2 = std::stoull(val.substr(0, val.size()-3));
            }
            else if (key == "key3") {
                auto first = parts[i].find('"');
                auto last  = parts[i].rfind('"');
                if (first==std::string::npos || last==first) return false;
                tmp.key3 = parts[i].substr(first+1, last-first-1);
            }
            else return false;
        }
    } catch(...) {
        return false;
    }
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& dst) {
    std::string line;
    while (true) {
        if (!std::getline(in, line))
            return in;
        DataStruct tmp;
        if (parseLine(line, tmp)) {
            dst = tmp;
            return in;
        }
    }
}

std::ostream& operator<<(std::ostream& os, DataStruct const& v) {
    std::ostream::sentry sent(os);
    if (!sent) return os;
    IOfmtGuard g(os);
    os << "(:key1 "
       << std::fixed << std::setprecision(1) << v.key1 << 'd'
       << ":key2 " << v.key2 << "ull"
       << ":key3 \"" << v.key3 << "\":)";
    return os;
}

bool cmpDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

} // namespace max
