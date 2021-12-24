#include "include.hpp"
#include <cstring>

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

typedef long value_type;

typedef void (*Instruction)(value_type& x, value_type param);

static const unordered_map<char, size_t> indices = {
	{'w', 0},
	{'x', 1},
	{'y', 2},
	{'z', 3}
};


struct ALU {
	ALU() {
		reset();
	}

	void reset() {
		for (int i = 0; i < 4; i++) {
			values[i] = 0;
		}
	}

	value_type& get(char c) {
		return values[indices.at(c)];
	}

	const value_type& get(char c) const {
		return values[indices.at(c)];
	}

	void print() const {
		size_t stacksize = 0;
		value_type z = values.back();
		while (z != 0) {
			z /= 26;
			stacksize++;
		}
		cout << "StackSize: " << stacksize << endl;
		for (int i = 0; i < 4; i++) {
			if (i != 0) {
				cout << ' ';
			}
			cout << values[i];
		}
		cout << endl;
	}

	std::array<value_type, 4> values;
};

static string model_string;
static size_t model_index = 0;
static long MAX_MODEL = 99999999999999;

void inputInstr(value_type& x, value_type param) {
	x = model_string[model_index] - '0';
	model_index++;
}
void addInstr(value_type& x, value_type param) {
	x = (x + param);
}
void mulInstr(value_type& x, value_type param) {
	x = (x * param);
}
void divInstr(value_type& x, value_type param) {
	assert(param != 0);
	x = (x / param);
}
void modInstr(value_type& x, value_type param) {
	assert(x >= 0 && param > 0);
	x = (x % param);
}
void eqlInstr(value_type& x, value_type param) {
	x = (x == param) ? 1 : 0;
}

void executeInstruction(vector<string> tokens, ALU& alu) {
	static const unordered_map<string, Instruction> instructions = {
		{"inp", inputInstr},
		{"add", addInstr},
		{"mul", mulInstr},
		{"div", divInstr},
		{"mod", modInstr},
		{"eql", eqlInstr}
	};

	size_t index = indices.at(tokens[1][0]);
	value_type param2;
	if (tokens.size() > 2) {
		if (isalpha(tokens[2][0])) {
			param2 = alu.get(tokens[2][0]);
		} else {
			param2 = std::stol(tokens[2]);
		}
	} else {
		param2 = 0;
	}
	// std::cout << tokens[0] << " " << tokens[1];
	// if (tokens.size() > 2) {
	// 	std::cout << " " << tokens[2];
	// }
	// cout << endl;
	instructions.at(tokens[0])(alu.values[index], param2);
	// alu.print();
}

vector<vector<string>> parseFile(ifstream& file) {
	vector<vector<string>> instr;
	string line;
	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		instr.push_back(util::splitString(line, ' '));
	}
	return instr;
}

string generateString(long& model_number) {
	string result = to_string(model_number);
	while (result.find('0') != std::string::npos) {
		model_number--;
		result = to_string(model_number);
	}
	return result;
}

int main(int argc, char *argv[]) {
	ALU alu;
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	auto instr = parseFile(file);
	file.close();

	model_index = 0;
	model_string = "96929994293996";
	model_string = "41811761181141";
	for (auto& i : instr) {
		if (i[0] == "inp" && model_index == 14) {
			break;
		}
		executeInstruction(i, alu);
	}
	alu.print();
	cout << "Processed: " << model_index << endl;
	cout << model_string << endl;
	return 0;
}
