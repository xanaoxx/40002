//Variant 5. key1 = DBL SCI; key2 = CHR LIT
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits>
#include <fstream>

#include "DataStruct.h"

const std::streamsize MAX_STREAM_SIZE = std::numeric_limits<std::streamsize>::max();

int main() {
    std::vector<DataStruct> data;
    //std::ifstream in("test.txt");
    std::istream& in = std::cin;
    while (!in.eof())
    {
        if (!in) {
            in.clear();
        }
        std::copy
        (
            std::istream_iterator< DataStruct >(in),
            std::istream_iterator< DataStruct >(),
            std::back_inserter(data)
        );
        if (!in)
        {
            in.ignore(MAX_STREAM_SIZE, '\n');
        }
    }
    std::sort(
        std::begin(data),
        std::end(data),
        compareDS
    );
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
