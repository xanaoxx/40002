#include "DataStruct.hpp"
#include <algorithm>

namespace dStruct
{

    std::istream& operator>>(std::istream& in, SllLit&& key) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            in.setstate(std::ios::failbit);
        }

        return in >> key.value >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
    }
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
           return in;
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
            in.setstate(std::ios::failbit);
        return in;
    }
     std::ostream& operator<<(std::ostream& out, const DataStruct& value) {
     std::ostream::sentry sentry(out);
     if (!sentry) {
         return out;
     }
     iofmtguard fmtguard(out);

     out << std::setprecision(1) << std::fixed << "(";
     out << ":key1 " << value.key1 << "d";
     out << ":key2 " << value.key2 << "ll";
     out << ":key3 \"" << value.key3 << "\":)";

     return out;
 }

}
