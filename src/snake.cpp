#include "stdafx.h"

#include "snake.hpp"

namespace snake {

void Snake::addSpeed(float speed)
{
	m_Speed += speed;
}

void Snake::removeSpeed(float speed)
{
	m_Speed -= speed;
}

int Snake::wrap(int value, int max) {
	return (value + max) % max;
}

void Snake::move(Direction direction)
{
	//gros pavé pour check si le joueur fait demi tour direct*
	//[TODO] rendre lisible et optimisé

	direction = m_currentDirection; // on ignore si le joueur fait un 180°
	Pos p = m_Body.front();

	switch (direction)
	{
	case Direction::left : p.x--; break;
	case Direction::up   : p.y--; break;
	case Direction::right: p.x++; break;
	case Direction::down : p.y++; break;
	}

	p.wrap();

	Index newHead = p;

	// check la collision on skip la tete sinon probleme
	for (size_t i = 1; i < m_Body.size(); ++i)
	{
		if (newHead == m_Body[i])
		{
			exit(0); // on quitte pour l'instant 
			//[TODO] faire un menu et un écran de fin
		}
	}

	for (int i = int(m_Body.size()) - 1; i > 0; --i)
	{
		m_Body[i] = m_Body[i - 1];
	}

	m_Body[0] = newHead;
}

} // namespace snake