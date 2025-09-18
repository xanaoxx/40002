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
}
