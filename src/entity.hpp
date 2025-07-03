#pragma once

namespace snake {

class Entity
{
public:
	virtual ~Entity() = default;

	virtual bool isCollidable() { return false; };

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