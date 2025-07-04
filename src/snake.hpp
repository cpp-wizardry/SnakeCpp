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
		, body{ pos }
	{
	}

	void grow(int count = 1);
	void move();

	Index head() const { return body.front(); }
	Index last() const { return body.back(); }
	int size() const { return int(body.size()); }

	float speed = 1.0f;
	int score = 0;

	Direction direction = Direction::right;

	std::vector<Index> body;
	std::vector<Index> bodyPrev;
};

} // namespace snake