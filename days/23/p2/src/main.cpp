#include "include.hpp"
#include <queue>
#include <limits>
#include <array>

using namespace std;

typedef long PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

struct State;
void printHistory(const State& s);

#define WIDTH 13
#define HEIGHT 7
// #define HISTORY

size_t getIndex(Point p) {
	return static_cast<size_t>(p.x + p.y * WIDTH);
}

unordered_map<char, long> targets = {
	{'A', 3},
	{'B', 5},
	{'C', 7},
	{'D', 9},
};

unordered_map<char, long> move_costs = {
	{'A', 1},
	{'B', 10},
	{'C', 100},
	{'D', 1000},
};

struct State {
#ifdef HISTORY
	State()
	: prev(nullptr), cost(0), mincost(0) {}
	State* prev;
#else
	State()
	: cost(0), mincost(0) {}
#endif

	bool operator==(const State& rhs) const {
		return grid == rhs.grid;
	}

	bool operator<(const State& rhs) const {
		return mincost > rhs.mincost;
	}

	void printExpansions(const vector<State>& expansions) const {
		std::cout << "Expansions:" << std::endl;
		for (const State& s : expansions) {
			s.print();
		}
		exit(0);
	}

/*
1. For every amphibean: check every possible legal move
*/
	vector<State> expand() const {
		// std::cout << "Expanding:" << std::endl;
		// print();
		vector<State> expansions;
		for (size_t y = 1; y < HEIGHT; y++) {
			for (size_t x = 1; x < WIDTH; x++) {
				if (isalpha(get(x, y))) {
					if (shouldMove(Point(x, y))) {
						moveAmphib(Point(x, y), expansions);
					}
				}
			}
		}
		// printExpansions(expansions);
		return expansions;
	}

	// Check if amph is already in the correct spot
	bool shouldMove(Point pos) const {
		return !(pos.x == targets[get(pos)]
			&& hasSameBelow(pos));
			// && (pos.y == 3 || (pos.y == 2 && get(Point(pos.x, 3)) == get(pos))));
	}

	bool hasSameBelow(Point pos) const {
		return hasSameBelow(pos, get(pos));
	}

	bool hasSameBelow(Point pos, char c) const {
		for (long y = pos.y + 1; y < HEIGHT - 1; y++) {
			if (get(pos.x, y) != c) {
				return false;
			}
		}
		return true;
	}

	typedef pair<Point, size_t> ExploreType;

	void moveAmphib(Point pos, vector<State>& expansions) const {
		PointSet explored;
		std::queue<ExploreType> to_explore;
		to_explore.push(ExploreType(pos, cost));
		explored.insert(pos);
		size_t move_cost = move_costs[get(pos)];
		while (!to_explore.empty()) {
			const ExploreType& p = to_explore.front();
			const vector<Point> adj = util::getEdgeAdjacentPoints(p.first);
			for (const Point& a : adj) {
				if (explored.count(a) > 0 || get(a) != '.') {
					continue;
				}
				to_explore.push(ExploreType(a, p.second + move_cost));
				explored.insert(a);
				if (isLegalMove(pos, a)) {
					addExpansion(pos, a, p.second + move_cost, expansions);
				}
			}
			to_explore.pop();
		}
	}

	bool isLegalMove(Point amph, Point target) const {
		static const std::set<long> forbidden = {3, 5, 7, 9};
		// is forbidden square
		if (target.y == 1 && forbidden.count(target.x)) {
			return false;
		}
		long target_x = targets[get(amph)];
		// is in hallway, or target is a room, and target is not the correct room
		if ((amph.y == 1 || target.y != 1) && target.x != target_x) {
			return false;
		}
		// is correct room, but there are non-matching characters below
		if (target.x == target_x && !hasSameBelow(target, get(amph))) {
			return false;
		}
		// if (target.x == target_x && target.y == 2 && get(target.x, 3) != get(amph)) {
		// 	return false;
		// }
		return true;
	}

	void addExpansion(Point amph, Point target, size_t move_cost, vector<State>& expansions) const {
		expansions.push_back(createExpansion(amph, target, move_cost));
	}

	State createExpansion(Point amph, Point target, size_t move_cost) const {
		State expanded(*this);
		expanded.grid[getIndex(target)] = get(amph);
		expanded.grid[getIndex(amph)] = '.';
		expanded.cost = move_cost;
		expanded.computeMinCost();
#ifdef HISTORY
		expanded.prev = new State(*this);
#endif
		return expanded;
	}

	void computeMinCost() {
		mincost = cost;
		int cnt = 0;
		for (size_t y = 0; y < HEIGHT; y++) {
			for (size_t x = 0; x < WIDTH; x++) {
				char c = get(x, y);
				if (isalpha(c)) {
					long target_x = targets[c];
					if ((long)x != target_x) {
						mincost += (std::abs((long)x - target_x) + y) * move_costs[c];
					} else if (y == 2 && c != get(x, y + 1)) {
						mincost += 4 * move_costs[c];
					}
				}
			}
		}
		mincost += computeMinRowMovement();
	}

	long computeMinRowMovement() {
		for (size_t y = 2; y < HEIGHT - 1; y++) {

		}
		return 0;
	}

	void print() const {
		cout << "Cost: " << cost << ", MinCost: " << mincost <<  endl;
		for (size_t y = 0; y < HEIGHT; y++) {
			for (size_t x = 0; x < WIDTH; x++) {
				cout << get(x, y);
			}
			cout << endl;
		}
		cout << endl;
	}

	char get(size_t x, size_t y) const {
		return get(Point(x, y));
	}

	char get(Point p) const {
		return grid[getIndex(p)];
	}

public:
	size_t cost;
	size_t mincost;
	std::array<char, WIDTH * HEIGHT> grid;
};

struct HashState {
	size_t operator()(const State& rhs) const {
		size_t hash = 1;
		int cnt = 0;
		for (size_t i = 0; i < rhs.grid.size() && cnt < 8; i++) {
			if (isalpha(rhs.grid[i])) {
				hash *= std::hash<size_t>()(i);
				cnt++;
			}
		}
		return hash;
	}
};

unordered_map<State, size_t, HashState> saved_states;

void parseLine(State& state, const string& line, size_t y) {
	for (size_t x = 0; x < WIDTH; x++) {
		if (x < line.size()) {
			state.grid[getIndex(Point(x, y))] = line[x];
		} else {
			state.grid[getIndex(Point(x, y))] = ' ';
		}
	}
}

bool isGoalState(const State& state) {
	for (size_t x = 3; x < 10; x += 2) {
		for (size_t y = 2; y < 4; y++) {
			char c = 'A' + (x - 3) / 2;
			if (state.grid[getIndex(Point(x, y))] != c) {
				return false;
			}
		}
	}
	return true;
}

#ifdef HISTORY
void printHistory(const State& s) {
	if (s.prev == nullptr) {
		s.print();
	} else {
		printHistory(*s.prev);
		s.print();
	}
}
#endif

size_t solve(const State& initial) {
	std::priority_queue<State> states;
	states.push(initial);
	size_t minscore = std::numeric_limits<size_t>::max();
	while (!states.empty()) {
		const State& state = states.top();
		if (state.mincost > saved_states[state]) {
			states.pop();
			continue;
		}
		if (isGoalState(state)) {
#ifdef HISTORY
			printHistory(state);
#else
			state.print();
#endif
			return state.cost;
		}
		vector<State> expansions = state.expand();
		// cout << "State: " << state.mincost << ", " << state.cost << std::endl;
		states.pop();
		for (const State& expansion: expansions) {
			if (saved_states.count(expansion) > 0 && expansion.mincost > saved_states[expansion]) {
				continue;
			}
			states.push(expansion);
			saved_states[expansion] = expansion.mincost;
		}
	}
	return minscore;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;

	State initial_state;
	size_t x = 0;
	size_t y = 0;
	while (getline(file, line)) {
		if (x == 0) {
			x = line.size();
		}
		parseLine(initial_state, line, y);
		y++;
	}
	initial_state.computeMinCost();
	saved_states[initial_state] = initial_state.mincost;
	size_t score = solve(initial_state);
	cout << score << endl;
	return 0;
}
