#include "figures.hpp"
#include <iterator>
#include <algorithm>
#include "delimiter.hpp"
std::istream& varfolomeeva::operator>>(std::istream& in, Point& point)
{
    std::istream::sentry guard(in);
    if (!guard)
    {
        return in;
    }
    using del = Delimiter;
    Point temp{ 0, 0 };
    in >> del{ '(' } >> temp.x >> del{ ';' } >> temp.y >> del{ ')' };
    if (in)
    {
        point = temp;
    }
    return in;
}
