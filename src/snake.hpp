#pragma once

#include "entity.hpp"

namespace snake {

class Snake : public Entity
{
public:
	Snake(float speed, int score, Index pos)
		: Entity(Kind::snake)
		, m_Speed(speed)
		, m_Score(score)
	{
		m_Body.push_back(pos);
	}

	void addSegment(int count)
	{
		for (int i = 0; i < count; ++i)
		{
			m_Body.push_back(m_Body.back()); //rajout d'un segment dans la derniere position
		}
	}

	void addSpeed(float speed);
	void removeSpeed(float speed);
	float getSpeed() const { return m_Speed; }
	
	void move(Direction direction);
	Index getPosition() const { return m_Body[0]; }
	
	void addScore(int score) { this->m_Score += score; }
	int getScore() { return m_Score; }

	std::vector<Index> getBody() const { return m_Body; }

	int wrap(int value, int max);

	bool getAlive() { return isAlive; }

	void setCurrentDirection(Direction direction) { this->m_currentDirection = direction; }
	Direction getCurrentDirection() const { return m_currentDirection; }
	void setNextDirection(Direction nextDirection) { this->m_nextDirection = nextDirection; }
	Direction getNextDirection() const { return m_nextDirection; }

	bool canChangeDirection = true;

private:
	float m_Speed = 3.0f*(0.3f*m_Length);//[TODO] rendre modulable si ajout difficulter
	int m_Length = 1;
	int position;
	const int _OFFSET = 16;
	int m_Score = 0;
	Direction m_currentDirection = Direction::left;
	Direction m_nextDirection = Direction::right;
	bool isAlive = true;

	std::vector<Index> m_Body;
};

} // namespace snake