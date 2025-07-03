#include "point.hpp"

namespace snake {

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
	this->Position = y * 16 + x; 
}

std::ostream& operator<<(std::ostream& os, const Point& /*point*/)
{
	//os << "Point(x: " << point.x << ", y: " << point.y << ", place: " << point.place << ")";
	os << 'o';
	return os;
}

} // namespace snake