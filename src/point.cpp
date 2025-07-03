#include "stdafx.h"

#include "point.hpp"

namespace snake {

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
	this->Position = y * 16 + x; 
}

} // namespace snake