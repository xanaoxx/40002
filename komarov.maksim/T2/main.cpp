#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.h"

using max::DataStruct;
using max::cmpDataStruct;

int main() {
    std::vector<DataStruct> v;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(v)
    );
    std::sort(v.begin(), v.end(), cmpDataStruct);
    std::copy(
        v.begin(), v.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
