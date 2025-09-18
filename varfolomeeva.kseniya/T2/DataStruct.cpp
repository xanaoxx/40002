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
    }

    std::istream& operator>>(std::istream& in, DBLLit&& key) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            in.setstate(std::ios::failbit);
        }

        return in >> key.value >> DelimiterIO{ 'd' };
    }

    std::istream& operator>>(std::istream& in, StrKey&& key) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            in.setstate(std::ios::failbit);
        }
        std::string tmp{};
        std::getline(in >> DelimiterIO{ '"' }, tmp, '"');
        if (in) {
            key.str = tmp;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, const StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        for (size_t i = 0; dest.expected[i] != '\0'; ++i)
        {
            char c{};
            in >> c;

            if (std::isalpha(c))
            {
                c = std::tolower(c);
            }

            if (c != dest.expected[i])
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        iofmtguard fmtguard(in);

        const unsigned int KEY_NUMBER = 3;
        size_t keyI = 0;

        in >> DelimiterIO{ '(' };

        for (size_t i = 0; i < KEY_NUMBER; ++i) {
            in >> StringIO{ ":key" } >> keyI;
            switch (keyI) {
            case 1: {
                in >> DBLLit{ value.key1 };
                break;
            }
            case 2: {
                in >> SllLit{ value.key2 };
                break;
            }
            case 3: {
                in >> StrKey{ value.key3 };
                break;
            }
            default: {
                in.setstate(std::ios::failbit);
            }
            }
        }

        in >> StringIO{ ":)" };
        return in;
    }

}
