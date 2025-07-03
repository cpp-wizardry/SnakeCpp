#include "stdafx.h"

#include "snake.hpp"

namespace snake {

void Snake::grow(int count)
{
	body.reserve(body.size() + count);
	auto const p = body.back();
	for (int i = 0; i < count; ++i)
	{
		body.push_back(p);
	}
}

void Snake::move()
{
	Pos p = head();

	switch (direction)
	{
	case Direction::left : p.x--; break;
	case Direction::up   : p.y--; break;
	case Direction::right: p.x++; break;
	case Direction::down : p.y++; break;
	}

	p.wrap();

	Index newHead = p;
	for (size_t i = 1; i < body.size(); ++i)
	{
		if (newHead == body[i])
		{
			exit(0); // todo: lose
		}
	}

	for (int i = size() - 1; i > 0; --i)
	{
		body[i] = body[i - 1];
	}

	body[0] = newHead;
}

} // namespace snake