#ifndef Snake_h
#define Snake_h

#include "PointStack.h"
#include "Point.h"
#include "PointList.h"
// Змейка 2-х цветная
class Snake
{
  private:
	  PointList* snakeBody;
	  unsigned short **weights;
	  Point*** points;
	  void ResetMatrix();
	  // Индекс головы змейки в массиве snakeBody.
	  unsigned short headPos;
	  // Поставить новую ягодку.
	  void ResetBerry();
	  // Получает наикротчайший путь до точки назначения.
	  PointList* GetPathToDest();
	  // Получает следующую точку для передвижения змейки.
	  Point* GetNextPoint();
	  // Инициализация класса.
	  void Initialize();
	  // Освобождение ресурсов.
	  void Dispose();
  public:
	Snake();
	Snake(unsigned short rowsCount, unsigned short columnsCount);
	// Точка назначения.
	Point* destPoint;
	// Матрица отображения.
	bool **matrix;
	unsigned short rowsCount;
	unsigned short columnsCount;
	// Передвигает змейку блтже к ягодке.
	void Move();
	bool isGameOver;
	void Reset();
};

#endif
