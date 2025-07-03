#include "stdafx.h"

#include "board.hpp"
#include "fruit.hpp"
#include "snake.hpp"

namespace snake {

int Board::m_BoardSize = 16;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}

Board::Board()
	: m_snake(5.0f, 1, 150)
	, m_fruit(0)
	, m_Board(m_BoardSize * m_BoardSize)
{
	m_Board[m_snake.getPosition()] = Entity::snake;
	spawnFruit();
}

Entity const& Board::getEntityAt(int index) const
{
	static Point pt {0, 0};

	switch (m_Board[index]) {
	case Entity::none: pt.setPosition(index); return pt;
	case Entity::snake: return m_snake;
	case Entity::fruit: return m_fruit;
	}
	SNAKE_UNREACHABLE;
}

void Board::moveSnake(int direction)
{
	std::vector<int> oldBody = m_snake.getBody();

	m_snake.move(direction);

	int newHeadPosition = m_snake.getPosition();

	if (newHeadPosition == m_fruit.getPosition())
	{
		spawnFruit();
		m_snake.addSegment(1);
		m_snake.addScore(m_fruit.getPoints());
	}

	for (int pos : oldBody)
	{
		m_Board[pos] = Entity::none;
	}

	for (int pos : m_snake.getBody())
	{
		m_Board[pos] = Entity::snake;
	}
}

int Board::getSnakeSegmentOrder(int index)
{
	const auto& body = getSnake().getBody(); 
	for (size_t i = 0; i < body.size(); ++i) {
		if (body[i] == index)
			return static_cast<int>(i); 
	}
	return -1;
}

Fruit const& Board::spawnFruit()
{
	std::vector<int> emptyPositions;
	for (int i = 0; i < int(m_Board.size()); i++)
	{
		if (m_Board[i] == Entity::none)
		{
			emptyPositions.push_back(i);
		}
	}

	assert(!emptyPositions.empty()); // todo: win?

	std::uniform_int_distribution<> distrib(0, int(emptyPositions.size()) - 1);

	int chosenIndex = emptyPositions[distrib(gen)];

	m_fruit.setPosition(chosenIndex);
	m_Board[chosenIndex] = Entity::fruit;
	return m_fruit;
}

} // namespace snake