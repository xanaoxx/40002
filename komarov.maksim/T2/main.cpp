#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "DataStruct.h"

int main()
{
    std::vector< max::DataStruct > data;

    std::copy(std::istream_iterator< max::DataStruct >(std::cin),
              std::istream_iterator< max::DataStruct >(),
              std::back_inserter(data));

    std::sort(data.begin(), data.end(), max::cmpDataStruct);

    std::copy(data.begin(), data.end(),
              std::ostream_iterator< max::DataStruct >(std::cout, "\n"));
}