#include "include.hpp"
#include "node.hpp"
#include "timer.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;


Node::Node(NodePointer parent, Type t)
: type(t), v(0), parent(parent), left(nullptr), right(nullptr) {}

NodePointer Node::base() {
	return NodePointer(this);
}

bool Node::isPair() const {
	return type == Node::PAIR;
}

size_t Node::calcMagnitude() const {
	size_t total = 0;
	if (type == VALUE) {
		return v;
	} else {
		return left->calcMagnitude() * 3 + right->calcMagnitude() * 2;
	}
}

void Node::clear() {
	if (type == PAIR) {
		left->clear();
		right->clear();
		delete left;
		delete right;
	}
	parent = nullptr;
}

NodePointer Node::previousValue(NodePointer x) const {
	NodePointer prev = x;
	while (x->parent != nullptr) {
		x = x->parent;
		if (prev == x->right) {
			x = x->left;
			assert(x);
			while (x->type == PAIR) {
				x = x->right;
			}
			return x;
		}
		prev = x;
	}
	return nullptr;
}

NodePointer Node::nextValue(NodePointer x) const {
	NodePointer prev = x;
	while (x->parent != nullptr) {
		x = x->parent;
		if (prev == x->left) {
			x = x->right;
			assert(x);
			while (x->type == PAIR) {
				x = x->left;
			}
			return x;
		}
		prev = x;
	}
	return nullptr;
}

void Node::explode() {
	assert(left->type == VALUE);
	assert(right->type == VALUE);
	NodePointer x = previousValue(base());
	NodePointer y = nextValue(base());
	if (x) {
		x->v += left->v;
	}
	if (y) {
		y->v += right->v;
	}
	delete left;
	delete right;
	type = VALUE;
	v = 0;
}

void Node::split() {
	assert(type == VALUE && v >= 10);
	type = PAIR;
	left = NodePointer(new Node(base(), VALUE));
	right = NodePointer(new Node(base(), VALUE));
	left->v = v / 2;
	right->v = v / 2 + v % 2;
}

void Node::print() const {
	if (type == PAIR) {
		cout << '[';
	} else {
		cout << v;
		return;
	}
	assert((void*)(left->parent) == (void*)this);
	assert((void*)(right->parent) == (void*)this);
	left->print();
	cout << ',';
	right->print();
	cout << ']';
	if (parent == nullptr) {
		cout << endl;
	}
}

NodePointer makeNode(NodePointer parent, Node::Type type) {
	return NodePointer(new Node(parent, type));
}

NodePointer addPair(NodePointer a, NodePointer b, NodePointer parent) {
	if (a == nullptr) {
		return b;
	} else if (b == nullptr) {
		return a;
	}
	assert(a->type == Node::PAIR && a->type == b->type);
	NodePointer x = makeNode(parent, Node::PAIR);
	x->left = a;
	a->parent = x;
	x->right = b;
	b->parent = x;
	return x;
}

NodePointer parseTree(const std::string& line) {
	std::stack<char> s;
	NodePointer root = makeNode(nullptr);
	for (std::size_t i = 0; i < line.size(); i++) {
		if (line[i] == '[') {
			s.push('[');
			root->type = Node::PAIR;
			root->left = makeNode(root);
			root = root->left;
		} else if (line[i] == ']') {
			assert(s.size() > 0);
			s.pop();
			root = root->parent;
		} else if (isdigit(line[i])) {
			root->type = Node::VALUE;
			assert(line[i + 1] == ',' || line[i + 1] == ']');
			root->v = line[i] - '0';
		} else if (line[i] == ',') {
			root = root->parent;
			root->right = makeNode(root);
			root = root->right;
		}
	}
	return root;
}

bool scanExplode(NodePointer x, size_t n) {
	if (x->type == Node::PAIR) {
		if (n >= 4 && !x->left->isPair() && !x->right->isPair()) {
			x->explode();
			return true;
		} else {
			return scanExplode(x->left, n + 1)
				|| scanExplode(x->right, n + 1);
		}
	}
	return false;
}

bool scanSplit(NodePointer x) {
	if (x->type == Node::PAIR) {
		return scanSplit(x->left)
			|| scanSplit(x->right);
	} else if (x->type == Node::VALUE) {
		if (x->v >= 10) {
			x->split();
			return true;
		}
	}
	return false;
}

void reduce(NodePointer root) {
	while (true) {
		if (scanExplode(root, 0)) {
			continue;
		}
		if (scanSplit(root)) {
			continue;
		}
		break;
	}
}

void clearTree(NodePointer root) {
	root->clear();
	delete root;
}

size_t PartOne(const vector<string>& lines) {
	NodePointer root = nullptr;
	for (const string& line : lines) {
		root = addPair(root, parseTree(line), nullptr);
		reduce(root);
	}
	size_t amount = root->calcMagnitude();
	clearTree(root);
	return amount;
}

size_t PartTwo(const vector<string>& lines) {
	size_t amount = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines.size(); j++) {
			if (j == i) {
				continue;
			}
			NodePointer root = addPair(parseTree(lines[i]), parseTree(lines[j]), nullptr);
			reduce(root);
			amount = std::max(amount, root->calcMagnitude());
			clearTree(root);
		}
	}
	return amount;
}

size_t benchmark(const vector<string>& lines, size_t (*f)(const vector<string>&)) {
	util::Timer timer;
	f(lines);
	timer.reset();
	size_t result = f(lines);
	std::cout << "Elapsed: " << timer.elapsed() << endl;
	std::cout << result << std::endl;
	return result;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	vector<string> lines;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	std::cout << "-- Part One --" << std::endl;
	benchmark(lines, PartOne);
	std::cout << "-- Part Two --" << std::endl;
	benchmark(lines, PartTwo);
	return 0;
}
