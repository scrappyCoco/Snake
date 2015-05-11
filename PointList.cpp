#include "PointList.h"
#define NULL 0

PointList::PointList(short size)
{
	buffer = new Point*[size];
	for (short i = 0; i < size; ++i)
	{
		buffer[i] = NULL;
	}
	bufferSize = size;
}


PointList::~PointList()
{
  if (buffer != NULL)
  {
    for (short i = 0; i < bufferSize; ++i)
    {
      if (buffer[i] != NULL)
      {
        buffer[i] = NULL;
      }
      delete buffer[i];
    }
    delete buffer;
  }
}

Point* PointList::Get(unsigned short pos)
{
	return buffer[pos];
}

void PointList::Set(unsigned short pos, Point* value)
{
	buffer[pos] = value;
}

