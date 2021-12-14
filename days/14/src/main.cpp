#include "include.hpp"

using namespace std;

typedef util::Point<int> Point;
typedef util::HashPoint<int> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHash;

typedef unordered_map<string, size_t> TemplateMap;

pair<string, string> ruleInsertion(const string& key, char rule) {
	std::string x, y;
	x.reserve(2);
	y.reserve(2);
	x.push_back(key.front());
	x.push_back(rule);
	y.push_back(rule);
	y.push_back(key.back());
	return make_pair(x, y);
}

size_t ascii[255];

void add(TemplateMap& temp, const string& key, size_t amount) {
	// if (temp.count(key) == 0) {
	// 	temp[key] = 0;
	// }
	temp[key] += amount;
}

TemplateMap pairInsertion(const TemplateMap& temp, const unordered_map<string, char>& rules) {
	TemplateMap result;

	for (const auto& it : temp) {
		char rule = rules.find(it.first)->second;
		auto p = ruleInsertion(it.first, rule);
		add(result, p.first, it.second);
		add(result, p.second, it.second);
		ascii[(int)rule] += it.second;
	}

	return result;
}

string pairInsertString(string base, const unordered_map<string, char>& rules) {
	string result;

	cout << "Num Pairs: " << base.size() - 1 << endl;
	for (size_t i = 0; i < base.size(); ++i) {
		if (i == base.size() - 1) {
			result.push_back(base.back());
		} else {
			string key = base.substr(i, 2);
			char value = rules.find(key)->second;
			ascii[(int)value] += 1;
			result.push_back(key.front());
			result.push_back(value);
		}
	}
	return result;
}

int main(int argc, char *argv[]) {
	TemplateMap templates;
	unordered_map<string, char> rules;
	for (int i = 0; i < 255; ++i) {
		ascii[i] = 0;
	}

	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());

	string base_temp;
	getline(file, base_temp);
	for (size_t i = 0; i < base_temp.size() - 1; ++i) {
		std::string x = base_temp.substr(i, 2);
		add(templates, x, 1);
		if (i == 0) {
			ascii[(int)x.front()] += 1;
		}
		ascii[(int)x.back()] += 1;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		rules[line.substr(0, 2)] = line.back();
	}
	file.close();

	for (int i = 0; i < 40; ++i) {
		// base_temp = pairInsertString(base_temp, rules);
		templates = pairInsertion(templates, rules);
	}

	size_t MIN = std::numeric_limits<size_t>::max();
	size_t MAX = 0;
	for (int i = 0; i < 255; ++i) {
		if (ascii[i] != 0) {
			MAX = std::max(ascii[i], MAX);
			MIN = std::min(ascii[i], MIN);
		}
	}
	cout << MAX << ", " << MIN << endl;
	cout << MAX - MIN << endl;
	return 0;
}
