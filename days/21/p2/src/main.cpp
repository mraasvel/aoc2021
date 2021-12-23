#include "include.hpp"
#include <stack>

using namespace std;

#define WINNING_SCORE 21

struct State {
	State(int p1, int p2)
	: p1(p1), p2(p2) {}

	bool operator==(const State& rhs) const {
		return p1 == rhs.p1 && p2 == rhs.p2;
	}

	int p1;
	int p2;
};

struct Score {
	Score(size_t p1 = 0, size_t p2 = 0)
	: p1(p1), p2(p2) {}

	bool playerOneWon() const {
		return p1 > p2;
	}

	bool isOver() const {
		return p1 >= WINNING_SCORE || p2 >= WINNING_SCORE;
	}

	size_t p1;
	size_t p2;
};

struct GameData {
	GameData() {}
	GameData(size_t cnt)
	: universe_count(cnt) {}

	GameData(size_t cnt, const std::vector<Score>& scores)
	: universe_count(cnt), scores(scores) {}
	size_t universe_count;
	std::vector<Score> scores;
};

// doesn't work cuz a state is uniquely identified by pos + scores
struct HashState {
	size_t operator()(const State& s) const {
		return util::HashPoint<int>()(util::Point<int>(s.p1, s.p2));
	}
};

typedef std::unordered_map<State, GameData, HashState> StateHash;

void simulate(State start) {
	StateHash DP = {
		{start, GameData(1, {Score(0, 0)})}
	};
	while (!DP.empty()) {
		next.swap(DP);
		next.clear();
	}
}

int main(int argc, char *argv[]) {

	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;

	getline(file, line);
	size_t p1 = std::stoi(line.substr(line.rfind(":") + 1));
	getline(file, line);
	size_t p2 = std::stoi(line.substr(line.rfind(":") + 1));
	file.close();

	std::cout << p1 << ", " << p2 << std::endl;
	simulate(State(p1, p2));
	return 0;
}
