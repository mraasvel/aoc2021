#include <vector>
#include <string>

namespace util {

	namespace _detail_ {
	
	static std::size_t find_first_of_set(const std::string& s,
										const std::string& set, std::size_t pos = 0) {
		while (pos < s.size() && set.find(s[pos]) == std::string::npos) {
			++pos;
		}

		return pos < s.size() ? pos : std::string::npos;
	}

	static std::size_t find_first_not_of_set(const std::string& s,
											const std::string& set, std::size_t pos = 0) {
		while (pos < s.size() && set.find(s[pos]) != std::string::npos) {
			++pos;
		}

		return pos < s.size() ? pos : std::string::npos;
	}

	}


std::vector<std::string> splitString(const std::string& input, const std::string& delim_set)
{
	std::vector<std::string> lines;

	std::size_t i = _detail_::find_first_not_of_set(input, delim_set);
	while (i < input.length()) {
		std::size_t end = _detail_::find_first_of_set(input, delim_set, i);
		lines.push_back(input.substr(i, end - i));
		i = _detail_::find_first_not_of_set(input, delim_set, end);
	}
	return lines;
}

std::vector<std::string> splitString(const std::string& input, char delim)
{
	std::vector<std::string> lines;

	size_t i = input.find_first_not_of(delim);
	while (i < input.length()) {
		size_t end = input.find_first_of(delim, i);
		lines.push_back(input.substr(i, end - i));
		i = input.find_first_not_of(delim, end);
	}
	return lines;
}

}
