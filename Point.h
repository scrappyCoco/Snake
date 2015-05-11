#ifndef Point_h
#define Point_h

// Точка в 2-х мерной координатнйо плоскости.
class Point
{
private:
	unsigned short _x;
	unsigned short _y;
public:
	Point();
	unsigned short GetX();
	unsigned short GetY();
	void SetX(unsigned short x);
	void SetY(unsigned short y);
	bool Equals(Point* p);
};

#endif
