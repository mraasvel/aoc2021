#include "include.hpp"
#include <stack>

using namespace std;

size_t actualValue(size_t v, size_t cap) {
	v = v % cap;
	if (v == 0) {
		v = cap;
	}
	return v;
}

struct Player {
	Player(size_t start, size_t score = 0)
	: pos(start), score(score) {}

	Player nextState(int roll) const {
		size_t pos = actualValue(this->pos + roll, 10);
		return Player(pos, score + pos);
	}

	size_t pos, score;
};

struct Game {
	Game(size_t p1_start, size_t p2_start, size_t num_universes = 1)
	: p1_turn(true), p1(p1_start), p2(p2_start), num_universes(num_universes) {}

	Game(const Player& p1, const Player& p2, size_t n, bool p1_turn)
	: p1_turn(p1_turn), p1(p1), p2(p2), num_universes(n) {}

	Game nextState(int roll) const {
		// Number of possibilities for (3, 4, 5, 6, 7, 8, 9) rolls
		static const int probabilities[] = {
			1, 3, 6, 7, 6, 3, 1
		};

		if (p1_turn) {
			return Game(p1.nextState(roll), p2, num_universes * probabilities[roll - 3], !p1_turn);
		} else {
			return Game(p1, p2.nextState(roll), num_universes * probabilities[roll - 3], !p1_turn);
		}
	}

	bool isOver() const {
		return p1.score >= 21 || p2.score >= 21;
	}

	bool p1Won() const {
		return p1.score > p2.score;
	}

	bool p1_turn;
	Player p1, p2;
	size_t num_universes;
};

void PartTwo(size_t p1_start, size_t p2_start) {
	std::stack<Game> states;
	states.push(Game(p1_start, p2_start));

	size_t p1_wins = 0;
	size_t p2_wins = 0;
	while (!states.empty()) {
		Game state = states.top();
		states.pop();
		for (int roll = 3; roll <= 9; roll++) {
			Game next = state.nextState(roll);
			if (next.isOver()) {
				if (next.p1Won()) {
					p1_wins += next.num_universes;
				} else {
					p2_wins += next.num_universes;
				}
			} else {
				states.push(next);
			}
		}
	}
	if (p1_wins > p2_wins) {
		std::cout << "Player One: " << p1_wins << std::endl;
	} else {
		std::cout << "Player Two: " << p2_wins << std::endl;
	}
}

int main(int argc, char *argv[]) {

	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;

	getline(file, line);
	size_t p1_start = line.back() - '0';
	getline(file, line);
	size_t p2_start = line.back() - '0';
	file.close();

	PartTwo(p1_start, p2_start);

	return 0;
}
