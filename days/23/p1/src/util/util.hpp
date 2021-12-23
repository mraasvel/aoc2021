#include "Point.hpp"
#include <vector>
#include <string>

namespace util {
std::vector<std::string> splitString(const std::string& input, const std::string& delim_set);
std::vector<std::string> splitString(const std::string& input, char delim);
}
