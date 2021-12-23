#pragma once
#include "util/Point.hpp"

class Scanner {
public:
	typedef util::Point<int> Point;
public:
	Scanner()
	: explored(false), next(0), delta(0, 0), orientation(0) {}

	std::vector<std::vector<Point>> getOrientations() const;
	void generateOrientations();
	std::vector<Point> getTranslations(const std::vector<Scanner>& scanners) const;

public:
	bool explored;
	size_t next;
	Point delta; // represents the position relative to scanner 0
	size_t orientation;
	std::vector<std::vector<Point>> orientations;
	std::vector<Point> beacons;
};

typedef Scanner::Point Point;
