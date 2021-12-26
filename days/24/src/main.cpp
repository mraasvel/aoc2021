#include "include.hpp"
#include <cstring>
#include <stack>

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

bool isValidModel(const std::vector<std::vector<std::string>>& instructions, const std::string& number);

struct OutputType {
	OutputType(int y, int index)
	: y(y), index(index) {}

	int y;
	int index;
};

std::ostream& operator<<(std::ostream& out, const OutputType& r) {
	out << "(" << r.index << ": " << r.y << ")";
	return out;
}

void printStack(std::stack<OutputType> stack) {
	std::cout << "Stack size: " << stack.size() << std::endl;
	while (!stack.empty()) {
		OutputType top = stack.top();
		std::cout << "  " << top << std::endl;
		stack.pop();
	}
}

/*
For each (inp): extract (x, y) values
So if std::abs(X + top) < 9, we can satisfy the if condition
The values of that position should then be saved as a range
*/

struct InputType {
	int x, y;
};

struct Range {
	Range(int min = 0, int max = 0)
	: min(min), max(max) {}
	int min, max;
};

// pair<TOP, CURRENT>
std::pair<Range, Range> calculateRanges(int x, int top) {
// TOP == CURRENT - (x + top)
// CURRENT == TOP + (x + top)
	int sum = x + top;
	std::pair<Range, Range> result(Range(10, 0), Range(10, 0));
	for (int i = 1; i <= 9; i++) {
		if (i + sum <= 9 && i + sum >= 1) {
			result.first.min = std::min(result.first.min, i);
			result.first.max = std::max(result.first.min, i);
		}
		if (i - sum <= 9 && i - sum >= 1) {
			result.second.min = std::min(result.second.min, i);
			result.second.max = std::max(result.second.min, i);
		}
	}
	return result;
}

bool canSolve(int x, int top) {
	return std::abs(x + top) < 9;
}

std::vector<Range> generateRanges(const std::vector<InputType>& inputs) {
	std::vector<Range> ranges(inputs.size());

	std::stack<OutputType> stack;
	for (std::size_t i = 0; i < inputs.size(); i++) {
		const InputType& input = inputs[i];
		int top = stack.empty() ? 0 : stack.top().y;
		if (canSolve(input.x, top)) {
			// Calculate ranges of topIndex and currentIndex
			std::pair<Range, Range> result = calculateRanges(input.x, top);
			ranges[stack.top().index] = result.first;
			ranges[i] = result.second;
			stack.pop();
		} else {
			std::cout << "Push: " << i << std::endl;
			stack.push(OutputType(input.y, i));
		}
	}
	for (std::size_t i = 0; i < ranges.size(); i++) {
		std::cout << char('A' + i) << ": " << ranges[i].min << " -> " << ranges[i].max << std::endl;
	}
	return ranges;
}

std::vector<InputType> parseInputs(const std::vector<std::vector<std::string>>& instructions) {
	std::vector<InputType> inputs;
	for (std::size_t i = 0; i < instructions.size(); i++) {
		if (instructions[i][0] == "inp") {
			inputs.push_back(InputType());
		} else if (instructions[i][0] == "add") {
			if (!isalpha(instructions[i][2][0])) {
				if (instructions[i][1] == "y") {
					inputs.back().y = std::stoi(instructions[i][2]);
				} else if (instructions[i][1] == "x") {
					inputs.back().x = std::stoi(instructions[i][2]);
				}
			}
		}
	}
	return inputs;
}

std::string generateModelString(const std::vector<std::vector<std::string>>& instructions) {
	std::vector<InputType> inputs = parseInputs(instructions);
	std::vector<Range> ranges = generateRanges(inputs);
	std::string model;
	for (const Range& range : ranges) {
		model.append(std::to_string(range.max));
	}
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

	std::string model_string = generateModelString(instructions);
	// std::string model_string = "96929994293996";
	// std::string model_string = "41811761181141";
	bool result = isValidModel(instructions, model_string);
	std::cout << model_string << " is valid: " << std::boolalpha << result << std::endl;
	return 0;
}
