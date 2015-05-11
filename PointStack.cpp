#include "PointStack.h"
#define NULL 0

PointStack::PointStack(short size)
{
  pos = -1;
  bufferSize = size;
  buffer = new Point*[size];
  for (short i = 0; i < size; ++i)
  {
    buffer[i] = NULL;
  }
}

PointStack::~PointStack()
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

void PointStack::Push(Point* point)
{
  if (pos + 1 < bufferSize)
  {
    ++pos;
    buffer[pos] = point;
  }
}

Point* PointStack::Pop()
{
  Point* result = NULL;
  if (pos >= 0)
  {
    result = buffer[pos];
    --pos;
  }
  return result;
}

short PointStack::GetPos()
{
  return pos;
}

