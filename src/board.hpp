#pragma once

#include "fruit.hpp"
#include "point.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	static int getBoardSize() { return m_BoardSize; };
	static void setBoardSize(int Size) { m_BoardSize = Size; } // todo: this is broken

	Board();

	Entity const& getEntityAt(int index) const;

	Snake& getSnake() { return m_snake; }
	void moveSnake(int direction);
	int getSnakeSegmentOrder(int index);

	Fruit const& spawnFruit();

private:
	static int m_BoardSize;

	Snake m_snake;
	Fruit m_fruit;

	enum class Entity {
		none,
		snake,
		fruit,
	};
	std::vector<Entity> m_Board;
};

} // namespace snake