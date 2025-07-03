#pragma once

#include "fruit.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	Board();

	void moveSnake();
	void spawnFruit();

	Snake snake;
	Fruit fruit;

	std::array<Entity::Kind, boardSize * boardSize> board;
};

} // namespace snake