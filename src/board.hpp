#pragma once

#include "fruit.hpp"
#include "point.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	static int getBoardSize() { return m_BoardSize; };
	static void setBoardSize(int Size) { m_BoardSize = Size; }

	Board();
	std::shared_ptr<Snake> getSnake() const;
	void moveSnake(int direction);
	const std::vector<std::shared_ptr<Entity>>& getEntities() const { return m_Board; }
	std::shared_ptr<Fruit> spawnFruit();
	std::shared_ptr<Entity> getEntityAt(int index) const
	{
		return m_Board[index];
	}
	int getSnakeSegmentOrder(int index) const;

private:
	static int m_BoardSize;
	std::vector<std::shared_ptr<Entity>>m_Board;
};

} // namespace snake