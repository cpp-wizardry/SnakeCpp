#pragma once
#include "Entity.h"
#include "stdafx.h"




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
