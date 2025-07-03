#pragma once
#include "stdafx.h"
#include "entity.h"

class Point : public Entity
{
public:
	Point(int x, int y);
	
	int getPosX() {return x;};
	int getPosY() {return y;};
	bool isCollidable() override { return false; };
	void render() const override {
		//system("color 0A");
		std::cout << "\033[32mo\033[0m";
	}

private:
	int x, y;
	int Position = 0;

	friend std::ostream& operator<<(std::ostream& os, const Point& point);
};