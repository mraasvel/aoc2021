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

/*
If the delta == 3
A = B + 3
Then A can be at most 6 (9 - 3)
And B has to be at minimum 4 (1 + 3)
Inverted for negative sums
*/
std::pair<Range, Range> calculateRanges(int x, int top) {
	std::pair<Range, Range> result(Range(10, 0), Range(10, 0));
	int sum = x + top;
	result.first.min = 1;
	result.first.max = 9 - std::abs(sum);
	result.second.min = 1 + std::abs(sum);
	result.second.max = 9;
	if (sum < 0) {
		return std::pair<Range, Range>(result.second, result.first);
	} else {
		return result;
	}
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
			stack.push(OutputType(input.y, i));
		}
	}
	std::cout << "Ranges" << std::endl;
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
