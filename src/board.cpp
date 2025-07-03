#include "stdafx.h"

#include "board.hpp"
#include "fruit.hpp"
#include "snake.hpp"

namespace snake {

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}

Board::Board()
	: m_snake(5.0f, 1, Index{150})
	, m_fruit(Index{0})
	, m_board(boardSize * boardSize)
{
	m_board[m_snake.getPosition().idx] = Entity::Kind::snake;
	spawnFruit();
}

void Board::moveSnake(Direction direction)
{
	auto oldBody = m_snake.getBody();

	m_snake.move(direction);
	if (m_snake.getPosition() == m_fruit.pos)
	{
		spawnFruit();
		m_snake.addSegment(1);
		m_snake.addScore(m_fruit.points);
	}

	for (auto pos : oldBody)
	{
		m_board[pos.idx] = Entity::Kind::none;
	}

	for (auto pos : m_snake.getBody())
	{
		m_board[pos.idx] = Entity::Kind::snake;
	}
}

int Board::getSnakeSegmentOrder(Index index)
{
	const auto& body = getSnake().getBody(); 
	for (size_t i = 0; i < body.size(); ++i) {
		if (body[i] == index)
			return static_cast<int>(i);
	}
	return -1;
}

void Board::spawnFruit()
{
	std::vector<Index> emptyPositions;
	for (int i = 0; i < int(m_board.size()); i++)
	{
		if (m_board[i] == Entity::Kind::none)
		{
			emptyPositions.push_back(Index{i});
		}
	}

	assert(!emptyPositions.empty()); // todo: win?

	std::uniform_int_distribution<> distrib(0, int(emptyPositions.size()) - 1);

	m_fruit.pos = emptyPositions[distrib(gen)];
	m_board[m_fruit.pos.idx] = Entity::Kind::fruit;
}

} // namespace snake