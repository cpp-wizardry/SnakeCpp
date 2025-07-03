#pragma once

namespace snake {

class Entity
{
public:
	int getPosition() const { return Position; }
	void setPosition(int pos) { Position = pos; }

protected:
	Entity(int pos)
		: Position(pos)
	{
	}

	int Position;
};

} // namespace snake