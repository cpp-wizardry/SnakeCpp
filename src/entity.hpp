#pragma once

namespace snake {

constexpr int boardSize = 16;

struct Index;

struct Pos {
	int x = 0;
	int y = 0;

	Pos(int x, int y) : x(x), y(y) {}
	Pos(int index) : x(index % boardSize), y(index / boardSize) {}
	Pos(Index index);

	void wrap() {
		x = (x + boardSize) % boardSize;
		y = (y + boardSize) % boardSize;
	}
};

struct Index {
	int idx = 0;

	Index(int idx) : idx(idx) {}
	Index(int x, int y) : idx(y * boardSize + x) {}
	Index(Pos p) : Index(p.x, p.y) {}

	constexpr bool operator==(Index const&) const noexcept = default;
};

inline Pos::Pos(Index index) : Pos(index.idx) {}

enum class Direction {
	left,
	up,
	right,
	down,
};

constexpr bool isColinear(Direction a, Direction b) noexcept
{
	unsigned const aa = unsigned(a) & 0b1;
	unsigned const bb = unsigned(b) & 0b1;
	return aa == bb;
}

class Entity
{
public:
	enum class Kind : char {
		none,
		snake,
		fruit,
	};

	//Kind const kind; // it's never used

protected:
	Entity(Kind /*kind*/)
		//: kind(kind)
	{
	}
};

} // namespace snake