#include "DataStruct.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>

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

std::istream& operator>>(std::istream& in, DataStruct& dst) {
    std::istream::sentry sent(in);
    if (!sent) return in;
    char c;
    in >> c; if (!in || c != '(')   { in.setstate(std::ios::failbit); return in; }
    in >> c; if (!in || c != ':')   { in.setstate(std::ios::failbit); return in; }
    DataStruct tmp;
    bool f1 = false, f2 = false, f3 = false;
    for (int field = 1; field <= 3; ++field) {
        in >> c; if (!in || c != ':') { in.setstate(std::ios::failbit); return in; }
        char buf[4] = {0};
        in.read(buf, 3);
        if (!in || std::string(buf) != "key") {
            in.setstate(std::ios::failbit);
            return in;
        }
        in >> c;
        if (!in || (c < '1' || c > '3')) {
            in.setstate(std::ios::failbit);
            return in;
        }
        char which = c;
        in.get(c);
        if (!in || c != ' ') {
            in.setstate(std::ios::failbit);
            return in;
        }
        switch (which) {
            case '1': {
                std::string tok;
                std::getline(in, tok, ':');
                if (tok.empty() || (tok.back()!='d' && tok.back()!='D')) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                try {
                    tmp.key1 = std::stod(tok.substr(0, tok.size()-1));
                } catch(...) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f1 = true;
                break;
            }
            case '2': {
                std::string tok;
                std::getline(in, tok, ':');
                if (tok.size()<4 ||
                   (tok.substr(tok.size()-3)!="ull" &&
                    tok.substr(tok.size()-3)!="ULL")) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                try {
                    tmp.key2 = std::stoull(tok.substr(0, tok.size()-3));
                } catch(...) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f2 = true;
                break;
            }
            case '3': {
                in.get(c);
                if (!in || c != '"') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                std::string val;
                std::getline(in, val, '"');
                tmp.key3 = val;
                in.get(c);
                if (!in || c != ':') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f3 = true;
                break;
            }
        }
    }
    in >> c;
    if (!in || c != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (f1 && f2 && f3) dst = tmp;
    else in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& os, DataStruct const& v) {
    std::ostream::sentry sent(os);
    if (!sent) return os;
    IOfmtGuard guard(os);
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
