#pragma once
#include "entity.hpp"
#include "stdafx.h"

namespace snake {

class Fruit : public Entity
{
public:
	Fruit(int pos) { this->Position = pos; }
	bool isCollidable() override { return true; };
	void render() const override {
		std::cout << "%"; 
	}
	int getPoints() { return points; };

private:
	int points = 1000;
};

} // namespace snake