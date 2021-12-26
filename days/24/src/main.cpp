#include "include.hpp"
#include <cstring>
#include <stack>

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

bool isValidModel(const std::vector<std::vector<std::string>>& instructions, const std::string& number);

struct Range {
	Range(int min = 0, int max = 0)
	: min(min), max(max), index(0) {}

	int min;
	int max;
	int index;
};

std::ostream& operator<<(std::ostream& out, const Range& r) {
	out << "(" << r.index << ": " << r.min << " -> " << r.max << ")";
	return out;
}

void printStack(std::stack<Range> stack) {
	std::cout << "Stack size: " << stack.size() << std::endl;
	while (!stack.empty()) {
		Range top = stack.top();
		std::cout << "  " << top << std::endl;
		stack.pop();
	}
}

std::string generateModelString(const std::vector<std::vector<std::string>>& instructions) {
	std::string model;
	std::stack<Range> stack;
	std::vector<Range> inputs;
	printStack(stack);
	return model;
}

std::vector<std::vector<std::string>> parseFile(std::ifstream& file) {
	std::vector<std::vector<std::string>> instr;
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		instr.push_back(util::splitString(line, ' '));
	}
	return instr;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	auto instructions = parseFile(file);
	file.close();

	std::string model = generateModelString(instructions);
	std::string model_string = "96929994293996";
	// std::string model_string = "41811761181141";
	bool result = isValidModel(instructions, model_string);
	std::cout << model_string << " is valid: " << std::boolalpha << result << std::endl;
	return 0;
}
