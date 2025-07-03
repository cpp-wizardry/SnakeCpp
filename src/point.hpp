#pragma once

#include "entity.hpp"

namespace snake {

class Point : public Entity
{
public:
	Point(int x, int y)
		: Entity(y*16 + x)
		, x(x)
		, y(y)
	{
	}
	
	bool isCollidable() override { return false; };

	int getPosX() { return x; };
	int getPosY() { return y; };

private:
	int x, y;
	int Position = 0;
};

} // namespace snake