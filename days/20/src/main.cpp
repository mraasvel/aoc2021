#include "include.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

struct Image {
	Image()
	: w(0), h(0) {}

	char get(Point p) const {
		return map[p.y][p.x];
	}

	bool validPoint(Point p) const {
		return p.x < w && p.y < h && p.x >= 0 && p.y >= 0;
	}

	void print() const {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				cout << get(Point(x, y));
			}
			cout << endl;
		}
		cout << endl;
	}

	template <typename Pred>
	void forEach(Pred f) {
		for (int y = 0; y < h; y++)  {
			for (int x = 0; x < w; x++) {
				f(map[y][x]);
			}
		}
	}

	void pad(const string& IEA, bool first = false) {
		char c = '.';
		int n = 128;
		w += n;
		h += n;
		for (int i = 0; i < n / 2; ++i) {
			map.insert(map.begin(), string(w, c));
		}
		for (string& s : map) {
			s.insert(0, string(n / 2, c));
			s.append(string(n / 2, c));
		}
		for (int i = 0; i < n / 2; ++i) {
			map.push_back(string(w, c));
		}
	}

	size_t countPixels() {
		size_t cnt = 0;
		forEach([&cnt](char c) {
			if (c == '#') {
				++cnt;
			}
		});
		return cnt;
	}

	vector<string> map;
	int w, h;
};

char calculatePixel(const Image& image, Point p, const string& IEA, bool first, bool even) {
	auto area = getArea(p);
	string binary;
	for (const Point& a : area) {
		if (!image.validPoint(a)) {
			if (first) {
				binary.push_back('0');
			} else if (!even || IEA.front() == '.') {
				binary.push_back(IEA[0] == '#' ? '1' : '0');
			} else {
				binary.push_back(IEA.back() == '#' ? '1' : '0');
			}
		} else {
			if (image.get(a) == '.') {
				binary.push_back('0');
			} else {
				binary.push_back('1');
			}
		}
	}
	return IEA[std::stoull(binary, NULL, 2)];
}

Image translateImage(const Image& image, const string& IEA) {
	Image result;

	string line;
	Point p(0, 0);
	static bool first = true;
	static bool even = true;
	while (image.validPoint(p)) {
		line.push_back(calculatePixel(image, p, IEA, first, even));
		p.x += 1;
		if (p.x == image.w) {
			result.map.push_back(line);
			line.clear();
			p.y += 1;
			p.x = 0;
		}
	}
	result.w = image.w;
	result.h = image.h;
	first = false;
	even = !even;
	return result;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());

	string IEA;

	Image image;
	std::string line;
	while (getline(file, line)) {
		if (line.empty()) {
			break;
		}
		IEA.append(line);
	}
	while (std::getline(file, line)) {
		if(line.empty()) {
			continue;
		}
		image.map.push_back(line);
	}
	file.close();
	image.h = image.map.size();
	image.w = image.map.front().size();
	image.pad(IEA, true);
	for (int i = 0; i < 50; ++i) {
		if (i == 2) {
			cout << "P1: " << image.countPixels() << endl;
		}
		image = translateImage(image, IEA);

	}
	cout << "P2: " << image.countPixels() << endl;
	return 0;
}
