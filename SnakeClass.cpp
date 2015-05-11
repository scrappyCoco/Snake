#include "Arduino.h"
#include "SnakeClass.h"

#define BUSY 0xFFFF
#define FREE 0xFFFE

Snake::Snake()
{
  Snake(8, 8);
}

void Snake::ResetMatrix()
{
  for (unsigned short rowNumber = 0; rowNumber < rowsCount; ++rowNumber)
  {
    // Инициализация.
    for (unsigned short columnNumber = 0; columnNumber < columnsCount; ++columnNumber)
    {
      matrix[rowNumber][columnNumber] = false;
    }
  }

  // Закрашиваем, там где змейка.
  Point* p = NULL;
  unsigned short i = 0;
  do
  {
    p = snakeBody->Get(i);
    if (p != NULL)
    {
      matrix[p->GetY()][p->GetX()] = true;
    }
    ++i;
  } 
  while (p != NULL);

  // Закрашиваем ягодку.
  if (destPoint != NULL)
  {
    matrix[destPoint->GetY()][destPoint->GetX()] = true;
  }
}

void Snake::Initialize()
{
  isGameOver = false;

  // Создаем матрицу.
  matrix = new bool*[rowsCount];
  weights = new unsigned short*[rowsCount];
  points = new Point**[rowsCount];
  for (unsigned short rowNumber = 0; rowNumber < rowsCount; ++rowNumber)
  {
    matrix[rowNumber] = new bool[columnsCount];
    weights[rowNumber] = new unsigned short[columnsCount];
    points[rowNumber] = new Point*[columnsCount];
    for (int colNumber = 0; colNumber < columnsCount; ++colNumber)
    {
      points[rowNumber][colNumber] = new Point;
      points[rowNumber][colNumber]->SetX(colNumber);
      points[rowNumber][colNumber]->SetY(rowNumber);
    }
  }

  unsigned short len = rowsCount * columnsCount;
  snakeBody = new PointList(len);
  for (unsigned short i = 0; i < len; ++i)
  {
    snakeBody->Set(i, NULL);
  }
  // Ставим змейку в исходную позицию.

  snakeBody->Set(0, points[0][0]);
  snakeBody->Set(1, points[0][1]);
  snakeBody->Set(2, points[0][2]);
  headPos = 2;

  destPoint = NULL;
  // Инициализация matrix.
  ResetMatrix();

  ResetBerry();

  // Update.
  ResetMatrix();
}

void Snake::Dispose()
{
  unsigned short len = columnsCount * rowsCount;

  //for (unsigned short i = 0; i < len; ++i)
  //{
  //	snakeBody->Set(i, NULL);
  //}

  for (unsigned short rowNumber = 0; rowNumber < rowsCount; ++rowNumber)
  {
    delete matrix[rowNumber];
    delete weights[rowNumber];
    for (unsigned short colNumber = 0; colNumber < columnsCount; ++colNumber)
    {
      delete points[rowNumber][colNumber];
    }
    delete points[rowNumber];
  }
  delete points;
  delete weights;
  delete matrix;
  delete snakeBody;
}

Snake::Snake(unsigned short rowsCount, unsigned short columnsCount)
{

  this->rowsCount = rowsCount;
  this->columnsCount = columnsCount;
  Initialize();
}


Point* Snake::GetNextPoint()
{
  // Проставляем веса по умолчанию.
  for (unsigned short rowNumber = 0; rowNumber < rowsCount; ++rowNumber)
  {
    for (unsigned short columnNumber = 0; columnNumber < columnsCount; ++columnNumber)
    {
      weights[rowNumber][columnNumber] = FREE;
    }
  }

  // Заняты клетки, там где тело змейки.
  for (unsigned short snakePeaceIndex = 0;; ++snakePeaceIndex)
  {
    Point* snakePeace = NULL;
    if ((snakePeace = snakeBody->Get(snakePeaceIndex)) == NULL)
    {
      break;
    }
    weights[snakePeace->GetY()][snakePeace->GetX()] = BUSY;
  }

  Point* currentPoint = snakeBody->Get(headPos);
  unsigned short currentWeight = 0;
  weights[currentPoint->GetY()][currentPoint->GetX()] = currentWeight;
  PointStack* currentStack = new PointStack(rowsCount * columnsCount);
  PointStack* futureStack = new PointStack(rowsCount * columnsCount);
  currentStack->Push(currentPoint);
  while (currentStack->GetPos() >= 0)
  {
    ++currentWeight;
    while (currentStack->GetPos() >= 0)
    {
      currentPoint = currentStack->Pop();
      unsigned short x, y;
      x = currentPoint->GetX();
      y = currentPoint->GetY();
      // ←
      if (x > 0 && weights[y][x - 1] == FREE)
      {
        weights[y][x - 1] = currentWeight;
        Point* futurePoint = points[y][x - 1];
        futureStack->Push(futurePoint);
      }
      // ↑
      if (y > 0 && weights[y - 1][x] == FREE)
      {
        weights[y - 1][x] = currentWeight;
        Point* futurePoint = points[y - 1][x];
        futureStack->Push(futurePoint);
      }
      // →
      if (x < columnsCount - 1 && weights[y][x + 1] == FREE)
      {
        weights[y][x + 1] = currentWeight;
        Point* futurePoint = points[y][x + 1];
        futureStack->Push(futurePoint);
      }
      // ↓
      if (y < rowsCount - 1 && weights[y + 1][x] == FREE)
      {
        weights[y + 1][x] = currentWeight;
        Point* futurePoint = points[y + 1][x];
        futureStack->Push(futurePoint);
      }
      //if (matrix[y][x] == false)
      //{
      //	delete currentPoint;
      //}
    }
    // Перекидываем из одного стека в другой.
    PointStack* tmpStackPtr = currentStack;
    currentStack = futureStack;
    futureStack = tmpStackPtr;
    // читсим стек.
    //debugWeight();
  }


  delete futureStack;
  delete currentStack;

  // побриаем путь из конечной точки к начальной.
  unsigned short x = destPoint->GetX();
  unsigned short y = destPoint->GetY();
  bool isBlocked = false;
  if (weights[destPoint->GetY()][destPoint->GetX()] != FREE)
  {
    currentWeight = weights[y][x];
  }
  else
  {
    isBlocked = true;
    currentWeight = 2;
    x = snakeBody->Get(headPos)->GetX();
    y = snakeBody->Get(headPos)->GetY();
  }

  Point* nextPoint = NULL;
  // Строим путь.
  //unsigned short resultLen = currentWeight;
  while (currentWeight > 1)
  {
    if (x > 0 && weights[y][x - 1] < currentWeight)
    {
      currentWeight = weights[y][x - 1];
      x = x - 1;
    }
    else if (x < rowsCount - 1 && weights[y][x + 1] < currentWeight)
    {
      currentWeight = weights[y][x + 1];
      x = x + 1;
    }
    else if (y > 0 && weights[y - 1][x] < currentWeight)
    {
      currentWeight = weights[y - 1][x];
      y = y - 1;
    }
    else if (y < columnsCount - 1 && weights[y + 1][x] < currentWeight)
    {
      currentWeight = weights[y + 1][x];
      y = y + 1;
    }
    if (isBlocked && currentWeight == 2)
    {
      nextPoint = NULL;
      break;
    }
    else
    {
      nextPoint = points[y][x];
      //result->Set(resultLen - currentWeight - 1, p);
    }
  }
  if (nextPoint == NULL && destPoint != NULL)
  {
    unsigned short xyDiff = 0;
    xyDiff = destPoint->GetX() > x ? destPoint->GetX() - x : x - destPoint->GetX();
    xyDiff += destPoint->GetY() > y ? destPoint->GetY() - y : y - destPoint->GetY();
    if (xyDiff == 0)
    {
      nextPoint = destPoint;
    }
  }

  return nextPoint == NULL ? NULL : nextPoint;
}

void Snake::Move()
{
  // Находим точку голову змейки.
  unsigned short len = rowsCount * columnsCount;
  Point* lastPoint = NULL;
  //unsigned short headPos = 0;
  for (unsigned short i = 0; i < len; ++i)
  {
    lastPoint = snakeBody->Get(i);
    if (lastPoint == NULL)
    {
      break;
    }
    headPos = i;
  }

  // Голова змейки.
  Point* headPoint = snakeBody->Get(headPos);
  // Куда перейдет голова змейки.
  Point* nextPoint = GetNextPoint();

  if (nextPoint != NULL && !destPoint->Equals(nextPoint))
  {
    // Двигаемся к ягодке.

    // Сдвигаем змейку.
    snakeBody->Set(0, NULL);
    for (unsigned short i = 0; i < headPos; ++i)
    {
      snakeBody->Set(i, snakeBody->Get(i + 1));
    }
    snakeBody->Set(headPos, nextPoint);
  }
  else
  {
    if (nextPoint == NULL)
    {
      // Game Over;
      isGameOver = true;
    }
    else
    {
      // Растем.
      Point* newHead = points[destPoint->GetY()][destPoint->GetX()];
      snakeBody->Set(headPos + 1, newHead);

      ResetBerry();
    }
  }

  // Перерисовываем.
  ResetMatrix();
  //if (isGameOver)
  //{
  //for (unsigned short i = 0; i < len; ++i)
  //{
  //	Point* p;
  //	p = snakeBody->Get(i);
  //	if (p != NULL)
  //	{
  //		snakeBody->Set(i, NULL);
  //	}
  //}
  //}
}

void Snake::ResetBerry()
{
  unsigned short x = rand() % (columnsCount - 1);
  unsigned short y = rand() % (rowsCount - 1);
  // Если уже занята ячейка, идем вправо.
  boolean finded = false;
  if (matrix[y][x] == true)
  {
    for (unsigned short row = y; row < rowsCount && !finded; ++row)
    {
      for (unsigned short col = x; col < columnsCount; ++col)
      {
        if (matrix[row][col] == false)
        {
          matrix[row][col] = true;
          x = col;
          y = row;
          finded = true;
          break;
        }
      }
    }
  }
  // Идем в другу сторону.
  if (!finded)
  {
    for (unsigned short row = y; row >= 0 && row < rowsCount && !finded; --row)
    {
      for (unsigned short col = x; col >= 0 && col < columnsCount; --col)
      {
        if (matrix[row][col] == false)
        {
          matrix[row][col] = true;
          x = col;
          y = row;
          finded = true;
          break;
        }
      }
    }
  }
  destPoint = points[y][x];
}

void Snake::Reset()
{
  Dispose();
  Initialize();
}

