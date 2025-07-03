#pragma once

#include "entity.hpp"

namespace snake {

class Snake : public Entity
{
public:
	Snake(float speed, int score, Index pos)
		: Entity(Kind::snake)
		, speed(speed)
		, score(score)
	{
		body.push_back(pos);
	}

	void grow(int count = 1);
	void move();

	Index head() const { return body[0]; }
	int size() const { return int(body.size()); }

	int getSnakeSegmentOrder(Index index) const
	{
		for (size_t i = 0; i < body.size(); ++i) {
			if (body[i] == index)
				return static_cast<int>(i);
		}
		return -1;
	}

	float speed = 1.0f;
	int score = 0;

	Direction direction = Direction::right;

	std::vector<Index> body;
};

} // namespace snake