#pragma once

#include <iostream>

namespace util {

template <typename T>
struct Point {
	Point(T x = 0, T y = 0)
	: x(x), y(y) {}

	Point operator+(const Point& rhs) {
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator-(const Point& rhs) {
		return Point(x - rhs.x, y - rhs.y);
	}

	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
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
	std::size_t operator()(const Point<T>& point) {
		return ((point.x + point.y) * (point.x + point.y + 1) / 2) + point.y;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Point<T>& rhs) {
	out << '(' << rhs.x << ", " << rhs.y << ')';
	return out;
}

}
