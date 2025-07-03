#pragma once

namespace snake {

constexpr int boardSize = 16;

struct Index;

struct Pos {
	int x;
	int y;

	Pos(int x, int y) : x(x), y(y) {}
	Pos(int index) : x(index % boardSize), y(index / boardSize) {}
	Pos(Index index);

	void wrap() {
		x = (x + boardSize) % boardSize;
		y = (y + boardSize) % boardSize;
	}
};

struct Index {
	int idx;

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

constexpr bool isOpposite(Direction dir1, Direction dir2) noexcept
{
	return (dir1 == Direction::left  && dir2 == Direction::right)
		|| (dir1 == Direction::right && dir2 == Direction::left )
		|| (dir1 == Direction::up    && dir2 == Direction::down )
		|| (dir1 == Direction::down  && dir2 == Direction::up   );
}

class Entity
{
public:
	enum class Kind : char {
		none,
		snake,
		fruit,
	};

	Kind const kind;

protected:
	Entity(Kind kind)
		: kind(kind)
	{
	}
};

} // namespace snake