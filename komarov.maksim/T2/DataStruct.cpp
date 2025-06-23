#include "DataStruct.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <stdexcept>

namespace max
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    char c = 0;
    in >> c;
    if (in && c != d.exp) in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleLiteralIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    std::string token;
    in >> token;
    if (token.size() < 2 || (token.back() != 'd' && token.back() != 'D'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    token.pop_back();
    try
    {
      d.ref = std::stod(token);
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, UnsignedLongLongLiteralIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    std::string token;
    in >> token;
    if (token.size() < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::string suffix = token.substr(token.size() - 3);
    std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                   [](char c){ return static_cast<char>(std::tolower(c)); });
    if (suffix != "ull")
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    token.erase(token.size() - 3);
    try
    {
      d.ref = std::stoull(token);
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    return std::getline(in >> DelimiterIO{ '"' }, d.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    std::string word;
    if ((in >> word) && word != d.exp) in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dst)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    DataStruct tmp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    in >> DelimiterIO{ '(' };
    std::string key;
    while (in >> DelimiterIO{ ':' } >> key)
    {
      if (key == ")")
      {
        in.putback(')');
        break;
      }
      if (key == "key1")
      {
        in >> DoubleLiteralIO{ tmp.key1 };
        hasKey1 = true;
      }
      else if (key == "key2")
      {
        in >> UnsignedLongLongLiteralIO{ tmp.key2 };
        hasKey2 = true;
      }
      else if (key == "key3")
      {
        in >> StringIO{ tmp.key3 };
        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    in >> DelimiterIO{ ')' };
    if (in && hasKey1 && hasKey2 && hasKey3)
      dst = tmp;
    else
      in.setstate(std::ios::failbit);
    return in;
  }

  IOFmtGuard::IOFmtGuard(std::basic_ios<char>& s)
    : stream_(s),
      width_(s.width()),
      fill_(s.fill()),
      prec_(s.precision()),
      flags_(s.flags())
  {}

  IOFmtGuard::~IOFmtGuard()
  {
    stream_.width(width_);
    stream_.fill(fill_);
    stream_.precision(prec_);
    stream_.flags(flags_);
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry s(out);
    if (!s) return out;
    IOFmtGuard guard(out);
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd'
        << ":key2 " << src.key2 << "ull"
        << ":key3 \"" << src.key3 << "\":)";
    return out;
  }
}
