#include <string>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <array>

typedef long value_type;

typedef void (*Instruction)(value_type& x, value_type param);

static const std::unordered_map<char, size_t> indices = {
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

	std::array<value_type, 4> values;
};

static std::string model_string;
static size_t model_index = 0;

static void inputInstr(value_type& x, value_type param) {
	x = model_string[model_index] - '0';
	model_index++;
}

static void addInstr(value_type& x, value_type param) {
	x = (x + param);
}

static void mulInstr(value_type& x, value_type param) {
	x = (x * param);
}

static void divInstr(value_type& x, value_type param) {
	assert(param != 0);
	x = (x / param);
}

static void modInstr(value_type& x, value_type param) {
	assert(x >= 0 && param > 0);
	x = (x % param);
}

static void eqlInstr(value_type& x, value_type param) {
	x = (x == param) ? 1 : 0;
}

static void executeInstruction(std::vector<std::string> tokens, ALU& alu) {
	static const std::unordered_map<std::string, Instruction> instructions = {
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
	instructions.at(tokens[0])(alu.values[index], param2);
}

bool isValidModel(const std::vector<std::vector<std::string>>& instructions, const std::string& number) {
	assert(number.size() == 14);
	model_string = number;
	model_index = 0;
	ALU alu;
	for (const auto& instruction : instructions) {
		executeInstruction(instruction, alu);
	}
	return alu.get('z') == 0;
}
