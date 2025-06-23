#include "DataStruct.h"

#include <iomanip>
#include <limits>
#include <cctype>

namespace          // ---------- маленькие утилиты, видимые только здесь ------
{
  struct Delim   { char exp;                    };
  struct Dbl     { double&              ref;    };
  struct Ull     { unsigned long long&  ref;    };
  struct Quoted  { std::string&         ref;    };
  struct Label   { const char*          exp;    };

  // 1. Обычный «прочитай-символ-и-проверь»
  std::istream& operator>>(std::istream& in, Delim d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    char c = 0;
    in >> c;
    if (in && c != d.exp) in.setstate(std::ios::failbit);
    return in;
  }

  // 2. double с суффиксом d / D
  std::istream& operator>>(std::istream& in, Dbl d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    in >> d.ref;
    char suf = 0;
    in >> suf;                                        // 'd' / 'D'
    if (in && suf!='d' && suf!='D') in.setstate(std::ios::failbit);
    return in;
  }

  // 3. unsigned long long + ull / ULL
  std::istream& operator>>(std::istream& in, Ull d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    in >> d.ref;
    char u = 0, l1 = 0, l2 = 0;
    in >> u >> l1 >> l2;
    bool ok = (u=='u'||u=='U')&&(l1=='l'||l1=='L')&&(l2=='l'||l2=='L');
    if (in && !ok) in.setstate(std::ios::failbit);
    return in;
  }

  // 4. строка в кавычках
  std::istream& operator>>(std::istream& in, Quoted q)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    char quote = 0;
    in >> quote;
    if (quote != '"') { in.setstate(std::ios::failbit); return in; }
    std::getline(in, q.ref, '"');
    return in;
  }

  // 5. точное ключевое слово
  std::istream& operator>>(std::istream& in, Label l)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    for (const char* p = l.exp; *p && in; ++p)
    {
      char c = 0; in >> c;
      if (c != *p) { in.setstate(std::ios::failbit); break; }
    }
    return in;
  }

  // 6. сохранение форматирования при выводе
  class IOfmtGuard
  {
    std::basic_ios<char>& s_;
    std::streamsize w_, p_;
    char fill_;
    std::ios::fmtflags f_;
  public:
    explicit IOfmtGuard(std::basic_ios<char>& s)
      : s_(s),
        w_(s.width()),
        p_(s.precision()),
        fill_(s.fill()),
        f_(s.flags()) {}
    ~IOfmtGuard()
    {
      s_.width(w_);
      s_.precision(p_);
      s_.fill(fill_);
      s_.flags(f_);
    }
  };

  // 7. пропустить пробелы **и** комментарии «# …»
  inline void skipUseless(std::istream& in)
  {
    static bool checkedForBom = false;            // смотрим только один раз
    if (!checkedForBom)
    {
      checkedForBom = true;
      std::istream::pos_type pos = in.tellg();
      char bom[3] = {0,0,0};
      in.read(bom, 3);
      if (!(bom[0] == char(0xEF) && bom[1] == char(0xBB) && bom[2] == char(0xBF)))
      {
        in.clear();                               // не BOM → вернуть позицию
        in.seekg(pos);
      }
    }

    while (true)
    {
      in >> std::ws;                              // пробелы / \r / \n / \t …
      if (in.peek() == '#')                       // строка-комментарий
      {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;                                 // и опять чистим пробелы
      }
      return;                                     // встретили «полезный» байт
    }
  }
} // unnamed namespace

// --------------------------- публичные операторы ----------------------------

std::istream& max::operator>>(std::istream& in, DataStruct& dst)
{
  skipUseless(in);                                        // комментарии + пустота

  std::istream::sentry s(in);
  if (!s) return in;

  DataStruct tmp;                                         // читаем во временный

  using D = Delim;   using L = Label; using Q = Quoted;
  using U = Ull;    using F = Dbl;   // (чтобы короче писать)

  in >> D{'('}
     >> D{':'} >> L{"key1"} >> F{tmp.key1}
     >> D{':'} >> L{"key2"} >> U{tmp.key2}
     >> D{':'} >> L{"key3"} >> Q{tmp.key3}
     >> D{':'} >> D{')'};

  if (in) dst = tmp;                                      // удача → переносим
  return in;
}

std::ostream& max::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry s(out);
  if (!s) return out;
  IOfmtGuard g(out);

  out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd'
      << ":key2 " << src.key2 << "ull"
      << ":key3 \"" << src.key3 << "\":)";
  return out;
}

bool max::cmpDataStruct(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1) return a.key1 < b.key1;
  if (a.key2 != b.key2) return a.key2 < b.key2;
  return a.key3.size() < b.key3.size();
}
