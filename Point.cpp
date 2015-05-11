#include "Point.h"


Point::Point()
{
	_x = 0;
	_y = 0;
}

unsigned short Point::GetX()
{
	return _x;
}
unsigned short Point::GetY()
{
	return _y;
}
void Point::SetX(unsigned short x)
{
	_x = x;
}
void Point::SetY(unsigned short y)
{
	_y = y;
}
bool Point::Equals(Point *p)
{
	return p->_x == _x && p->_y == _y;
}
