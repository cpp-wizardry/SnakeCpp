#pragma once

#include "fruit.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	Board();

	Entity::Kind getEntityAt(Index index) const { return m_board[index.idx]; }

	Snake& getSnake() { return m_snake; }
	void moveSnake(Direction direction);
	int getSnakeSegmentOrder(Index index);

	void spawnFruit();

private:
	Snake m_snake;
	Fruit m_fruit;

	std::vector<Entity::Kind> m_board;
};

} // namespace snake