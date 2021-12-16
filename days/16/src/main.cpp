#include "include.hpp"

using namespace std;

typedef size_t PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

static const unordered_map<char, string> hextobin = {
	{'0', "0000"},
	{'1', "0001"},
	{'2', "0010"},
	{'3', "0011"},
	{'4', "0100"},
	{'5', "0101"},
	{'6', "0110"},
	{'7', "0111"},
	{'8', "1000"},
	{'9', "1001"},
	{'A', "1010"},
	{'B', "1011"},
	{'C', "1100"},
	{'D', "1101"},
	{'E', "1110"},
	{'F', "1111"}
};

struct Packet {
	size_t version;
	size_t type;
	size_t length_type;
	size_t value;
	vector<Packet> packets;

	bool operator<(const Packet& rhs) const {
		return value < rhs.value;
	}
};

Packet parsePacket(const std::string& binary);

size_t ip = 0;
size_t total_version = 0;

std::string parseHex(const std::string& line) {
	std::string result;
	for (char c : line) {
		result.append(hextobin.at(c));
	}
	return result;
}

size_t btoi(const std::string& binary, size_t len) {
	return std::stoull(binary.substr(ip, len), NULL, 2);
}

size_t btoi(const std::string& binary) {
	return std::stoull(binary, NULL, 2);
}

size_t parseVersion(const std::string& binary) {
	size_t version = btoi(binary, 3);
	ip += 3;
	total_version += version;
	return version;
}

size_t parseType(const std::string& binary) {
	size_t type = btoi(binary, 3);
	ip += 3;
	return type;
}

void parseLiteral(const std::string& binary, Packet& packet) {
	std::string literal;
	do {
		literal.append(binary.substr(ip + 1, 4));
		ip += 5;
	} while (binary[ip - 5] != '0');
	packet.value = btoi(literal);
}

size_t sum(const vector<Packet>& packets) {
	size_t s = 0;
	for (const Packet& packet : packets) {
		s += packet.value;
	}
	return s;
}

size_t product(const vector<Packet>& packets) {
	size_t p = 1;
	for (const Packet& packet : packets) {
		p *= packet.value;
	}
	return p;
}

size_t minimum(const vector<Packet>& packets) {
	return std::min_element(packets.begin(), packets.end())->value;
}

size_t maximum(const vector<Packet>& packets) {
	return std::max_element(packets.begin(), packets.end())->value;
}

size_t greaterThan(const vector<Packet>& packets) {
	return packets[0].value > packets[1].value ? 1 : 0;
}

size_t lessThan(const vector<Packet>& packets) {
	return packets[0].value < packets[1].value ? 1 : 0;
}

size_t equalTo(const vector<Packet>& packets) {
	return packets[0].value == packets[1].value ? 1 : 0;
}


void parseOperator(const std::string& binary, Packet& packet) {
	vector<Packet> packets;
	packet.length_type = binary[ip] - '0';
	ip++;
	if (packet.length_type == 0) {
		size_t length = btoi(binary, 15);
		ip += 15;
		size_t start = ip;
		while (ip - start != length) {
			packets.push_back(parsePacket(binary));
		}
	} else {
		size_t length = btoi(binary, 11);
		ip += 11;
		while (length > 0) {
			packets.push_back(parsePacket(binary));
			--length;
		}
	}

	switch (packet.type) {
		case 0:
			packet.value = sum(packets);
			break;
		case 1:
			packet.value = product(packets);
			break;
		case 2:
			packet.value = minimum(packets);
			break;
		case 3:
			packet.value = maximum(packets);
			break;
		case 5:
			assert(packets.size() == 2);
			packet.value = greaterThan(packets);
			break;
		case 6:
			assert(packets.size() == 2);
			packet.value = lessThan(packets);
			break;
		case 7:
			assert(packets.size() == 2);
			packet.value = equalTo(packets);
			break;
	}
}

Packet parsePacket(const std::string& binary) {
	Packet packet;

	packet.version = parseVersion(binary);
	packet.type = parseType(binary);
	switch (packet.type) {
		case 4:
			parseLiteral(binary, packet);
			break;
		default:
			parseOperator(binary, packet);
			break;
	}
	return packet;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	getline(file, line);
	std::string binary = parseHex(line);
	file.close();

	Packet packet = parsePacket(binary);
	cout << "P1: " << total_version << endl;
	cout << "P2: " << packet.value << endl;
	return 0;
}
