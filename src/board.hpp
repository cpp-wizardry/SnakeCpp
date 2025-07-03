#pragma once

#include "fruit.hpp"
#include "point.hpp"
#include "snake.hpp"

namespace snake {

class Board
{
public:
	Board();
	void renderBoard();
	static int getBoardSize() { return m_BoardSize; };
	static void setBoardSize(int Size) { m_BoardSize = Size; }
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
	std::vector<std::shared_ptr<Entity>>m_Board;
	static int m_BoardSize;
};

} // namespace snake