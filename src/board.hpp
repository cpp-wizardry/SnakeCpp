#pragma once

#include "fruit.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	Board();

	Entity::Kind getEntityAt(Index index) const { return board[index.idx]; }

	void moveSnake();
	void spawnFruit();

	Snake snake;
	Fruit fruit;

	std::array<Entity::Kind, boardSize * boardSize> board;
};

} // namespace snake