#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>
#include <vector>
typedef std::vector<std::vector<bool>> axis;
namespace Helpers {
std::pair<axis, axis> GenerateMaze(const int dimension);
std::vector<std::vector<char>> GenerateMaze2(size_t);
}  // namespace Helpers

#endif  // HELPERS_HPP
