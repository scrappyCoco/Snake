#ifndef List_h
#define List_h

#include "Point.h"

class PointList
{
private:
	Point **buffer;
public:
	short bufferSize;
	PointList(short size);
	~PointList();
	Point* Get(unsigned short pos);
	void Set(unsigned short pos, Point* value);
};

#endif
