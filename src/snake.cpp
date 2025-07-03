#include "snake.h"

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

void Snake::move(int direction)
{
	//gros pavé pour check si le joueur fait demi tour direct*
	//[TODO] rendre lisible et optimisé

		direction = m_currentDirection; // on ignore si le joueur fait un 180°
	int head = m_Body.front();
	int row = head / 16;
	int col = head % 16;

	switch (direction)
	{
	case 1: col--; break;
	case 2: row--; break;
	case 3: col++; break;
	case 4: row++; break;
	}

	row = wrap(row, 16);
	col = wrap(col, 16);

	int newHead = row * 16 + col;

	// check la collision on skip la tete sinon probleme
	for (size_t i = 1; i < m_Body.size(); ++i)
	{
		if (newHead == m_Body[i])
		{
			std::cout << "Parti fini" << "\n";
			exit(0); // on quitte pour l'instant 
			//[TODO] faire un menu et un écran de fin
		}
	}

	for (int i = int(m_Body.size()) - 1; i > 0; --i)
	{
		m_Body[i] = m_Body[i - 1];
	}

	m_Body[0] = newHead;
	Position = newHead;

}

std::ostream& operator<<(std::ostream& os, const Snake& snake)
{
	os << "~";
	return os;
}
