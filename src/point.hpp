#pragma once

#include "entity.hpp"

namespace snake {

class Point : public Entity
{
public:
	Point(int x, int y);
	
	int getPosX() {return x;};
	int getPosY() {return y;};
	bool isCollidable() override { return false; };
	void render() const override {
	}

private:
	int x, y;
	int Position = 0;

	friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

} // namespace snake