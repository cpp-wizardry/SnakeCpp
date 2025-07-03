#pragma once

#include "entity.hpp"

namespace snake {

class Fruit : public Entity
{
public:
	Fruit(Index pos)
		: Entity(Kind::fruit)
		, pos(pos)
	{
	}

	Index pos;
	static constexpr int points = 1000;
};

} // namespace snake