#pragma once

#include <iostream>
#include <vector>

namespace util {

template <typename T>
struct Point {
	Point(T x = 0, T y = 0)
	: x(x), y(y) {}

	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}

	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Point& rhs) const {
		return !(*this == rhs);
	}

	bool operator>(const Point& rhs) const {
		return (rhs < *this) && !(*this < rhs);
	}

	bool operator<(const Point& rhs) const {
		return x < rhs.x || (x == rhs.x && y < rhs.y);
	}

	T x, y;
};

/*
https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
*/
template <typename T>
struct HashPoint {
	std::size_t operator()(const Point<T>& point) const {
		return ((point.x + point.y) * (point.x + point.y + 1) / 2) + point.y;
	}
};

template <typename T>
std::vector<Point<T> > getArea(Point<T> p) {
	const std::vector<Point<T> > adj {
		Point<T>(p.x - 1, p.y - 1),
		Point<T>(p.x, p.y - 1),
		Point<T>(p.x + 1, p.y - 1),

		Point<T>(p.x - 1, p.y),
		Point<T>(p.x, p.y),
		Point<T>(p.x + 1, p.y),
		Point<T>(p.x - 1, p.y + 1),
		Point<T>(p.x, p.y + 1),
		Point<T>(p.x + 1, p.y + 1),
	};

	return adj;
}

template <typename T>
std::vector<Point<T> > getEdgeAdjacentPoints(Point<T> p) {
	const std::vector<Point<T> > adj {
		Point<T>(p.x - 1, p.y),
		Point<T>(p.x + 1, p.y),
		Point<T>(p.x, p.y - 1),
		Point<T>(p.x, p.y + 1),
	};
	return adj;
}

template <typename T>
std::vector<Point<T> > getAdjacentPoints(Point<T> p) {
	std::vector<Point<T> > adj(getEdgeAdjacentPoints(p));

	adj.push_back(Point<T>(p.x + 1, p.y + 1));
	adj.push_back(Point<T>(p.x - 1, p.y + 1));
	adj.push_back(Point<T>(p.x + 1, p.y - 1));
	adj.push_back(Point<T>(p.x - 1, p.y - 1));
	return adj;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Point<T>& rhs) {
	out << '(' << rhs.x << ", " << rhs.y << ')';
	return out;
}

}
