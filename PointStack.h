#ifndef Stack_h
#define Stack_h

#include "Point.h"

class PointStack
{
private:
	short bufferSize;
	Point **buffer;
	short pos;
public:
	PointStack(short size);
	~PointStack();
	void Push(Point* point);
	Point* Pop();
	short GetPos();
};
#endif

