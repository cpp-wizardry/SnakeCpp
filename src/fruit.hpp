#pragma once

#include "entity.hpp"

namespace snake {

class Fruit : public Entity
{
public:
	Fruit(int pos)
		: Entity(pos)
	{
	}

	bool isCollidable() override { return true; };

	int getPoints() { return points; };

private:
	int points = 1000;
};

} // namespace snake