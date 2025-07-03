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
	: snake(5.0f, 1, Index{150})
	, fruit(Index{0})
{
	for (auto pos : snake.body)
	{
		board[pos.idx] = Entity::Kind::snake;
	}
	spawnFruit();
}

void Board::moveSnake()
{
	auto last = snake.body.back();

	snake.move();
	if (snake.head() == fruit.pos)
	{
		snake.score += fruit.points;
		spawnFruit();
		snake.grow();
	}

	board[last.idx] = Entity::Kind::none;
	board[snake.body.back ().idx] = Entity::Kind::snake; // in case snake.body.back() == last (like grow)
	board[snake.body.front().idx] = Entity::Kind::snake;
}

void Board::spawnFruit()
{
	std::vector<Index> emptyPositions;
	for (int i = 0; i < int(board.size()); i++)
	{
		if (board[i] == Entity::Kind::none)
		{
			emptyPositions.push_back(Index{i});
		}
	}

	if (emptyPositions.empty())
		exit(0); // todo: win?

	fruit.pos = emptyPositions[std::uniform_int_distribution<>(0, int(emptyPositions.size()) - 1)(gen)];
	board[fruit.pos.idx] = Entity::Kind::fruit;
}

} // namespace snake