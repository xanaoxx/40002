#include "commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include "streamGuard.hpp"
#include<string>

struct HasNumOfVertexes
{
    size_t vertexNum;
    explicit HasNumOfVertexes(size_t num);
    bool operator()(const varfolomeeva::Polygon& poly) const;
};

std::pair< varfolomeeva::Point, varfolomeeva::Point > makePair(const varfolomeeva::Point& p1, const varfolomeeva::Point& p2);
double shoelaceFormula(double acc, const std::pair< varfolomeeva::Point, varfolomeeva::Point >& points);
double sumArea(double acc, const varfolomeeva::Polygon& poly);
double calcArea(const varfolomeeva::Polygon& poly);
bool isEven(const varfolomeeva::Polygon& poly);
bool isOdd(const varfolomeeva::Polygon& poly);
double calcEvenArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcOddArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcMeanArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcNumVertexArea(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum);
bool compareVertexes(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2);
double getMaxArea(const std::vector< varfolomeeva::Polygon >& poly);
size_t getMaxVertexes(const std::vector< varfolomeeva::Polygon >& poly);
double getMinArea(const std::vector< varfolomeeva::Polygon >& poly);
size_t getMinVertexes(const std::vector< varfolomeeva::Polygon >& poly);
size_t countEven(const std::vector< varfolomeeva::Polygon >& poly);
size_t countOdd(const std::vector< varfolomeeva::Polygon >& poly);
size_t countNum(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum);
bool isIdentical(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2, size_t& counter);
bool isSame(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2);
bool isSamePoint(const varfolomeeva::Point& point, int x, int y, const varfolomeeva::Polygon& poly);

struct MaxSeqCounter
{
    size_t currentSeqLength;
    size_t maxSeqLength;
    bool operator()(const varfolomeeva::Polygon& poly, const varfolomeeva::Polygon& data);
};
