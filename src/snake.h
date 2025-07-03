#pragma once
#include "stdafx.h"
#include "entity.h"


class Snake : public Entity
{
public:
	Snake(float speed, int score, int startPosition)
		: m_Speed(speed), m_Score(score)
	{
		m_Body.push_back(startPosition);
		Position = startPosition; 
		isAlive = true;
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

	float getSpeed() { return m_Speed; };
	
	void move(int direction);
	
	void render() const override {
		//system("color 0C");
		std::cout << "\033[34m~\033[0m"; //ANSI code color pour changement de la couleur dans terminal
	}
	
	void setPosition(int pos) { position = pos; };
	
	int getPosition() const { return m_Body[0]; }
	
	bool isCollidable() override { return true; };
	
	void addScore(int score) { this->m_Score += score; };
	
	int getScore() { return m_Score; };
	std::vector<int> getBody() { return m_Body; };
	int wrap(int value, int max);
	bool getAlive() { return isAlive; };
	void setCurrentDirection(int direction) {this->m_currentDirection = direction;};
	int getCurrentDirection() { return m_currentDirection; };
	void setNextDirection(int nextDirection) {this->m_nextDirection = nextDirection; };
	int getNextDirection() { return m_nextDirection; };
	bool isOpposite(int dir1, int dir2) {
		return (dir1 == 1 && dir2 == 3) || (dir1 == 3 && dir2 == 1) ||
			(dir1 == 2 && dir2 == 4) || (dir1 == 4 && dir2 == 2);
	}


	bool canChangeDirection = true;


private:

	float m_Speed = 3.0f*(0.3f*m_Length);//[TODO] rendre modulable si ajout difficulter
	int m_Length = 1;
	int position;
	const int _OFFSET = 16;
	int m_Score = 0;
	int m_currentDirection = 1;
	int m_nextDirection = 3;
	bool isAlive;




	std::vector<int> m_Body;//liste des segments du serpent
	friend std::ostream& operator<<(std::ostream& os, const Snake& snake);



};






