#include "include.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

size_t actualValue(size_t v, size_t cap) {
	v = v % cap;
	if (v == 0) {
		v = cap;
	}
	return v;
}

struct Dice {

	Dice()
	: cap(100) {
		for (size_t i = 0; i < cap; i++) {
			values.push_back(i + 1);
			for (size_t j = 1; j < 3; j++) {
				values.back() += actualValue(i + j + 1, cap);
			}
		}
	}

	size_t operator[](size_t index) const {
		return values[index % cap];
	}

	size_t cap;
	vector<size_t> values;
};

struct Player {
	Player()
	: score(0), pos(0) {}

	void move(const Dice& dice, size_t turn) {
		pos = actualValue(pos + dice[turn], 10);
		score += pos;
	}

	size_t score;
	size_t pos;
};

struct Game {
	Game(size_t turn = 0, bool p1 = true)
	: turn(turn), p1(p1) {}

	Game(const Game& rhs)
	: turn(rhs.turn), p1(rhs.p1), a(rhs.a), b(rhs.b) {}

	bool isOver() const {
		return a.score >= 1000 || b.score >= 1000;
	}

	size_t computePartOne() const {
		return turn * std::min(a.score, b.score);
	}

	void doTurn() {
		if (p1) {
			a.move(dice, turn);
		} else {
			b.move(dice, turn);
		}
		turn += 3;
		p1 = !p1;
	}

	void playGame() {
		while (!isOver()) {
			doTurn();
		}
	}

	size_t turn;
	bool p1;
	Player a;
	Player b;
	Dice dice;
};

int main(int argc, char *argv[]) {
	Game game;
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	getline(file, line);
	game.a.pos = line.back() - '0';
	getline(file, line);
	game.b.pos = line.back() - '0';
	file.close();

	game.playGame();
	cout << game.computePartOne() << endl;
	return 0;
}
