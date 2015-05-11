#include "SnakeClass.h"

int rows[] = { 
  2, 3, 4, 5, 6, 7, 8, 9 }; // КАТОДЫ
int cols[] = {
  11, 12, 14, 15, 16, 17, 18, 19}; // АНОДЫ

Snake* snake;

void setup()
{
  for (int pinNumber = 0; pinNumber < 8; ++pinNumber)
  {
    pinMode(rows[pinNumber], OUTPUT);
    pinMode(cols[pinNumber], OUTPUT);

    digitalWrite(rows[pinNumber], HIGH);
    digitalWrite(cols[pinNumber], HIGH);
  }

  snake = new Snake(8,8);
}

int elapsedTime = 0;
void loop()
{
  if (snake->isGameOver)
  {
    snake->Reset();
  }
  else
  {
    // Для моргания ягодки.
    for (unsigned short columnNumber = 0; columnNumber < snake->columnsCount; ++columnNumber)
    {
      refreshCol();
      digitalWrite(cols[columnNumber], HIGH);
      for (unsigned short rowNumber = 0; rowNumber < snake->rowsCount; ++rowNumber)
      {
        refreshRow();
        if (snake->destPoint->GetX() == columnNumber
          && snake->destPoint->GetY() == rowNumber
          && elapsedTime % 25 == 1)
        {
          digitalWrite(rows[rowNumber], snake->matrix[rowNumber][columnNumber] ? HIGH : LOW);
        }
        else
        {
          digitalWrite(rows[rowNumber], snake->matrix[rowNumber][columnNumber] ? LOW : HIGH);
        }					
      }
      refreshRow();
    }
    
    if (elapsedTime > 50)
    {
      elapsedTime = 0;
      snake->Move();
    }
    else
    {
      ++elapsedTime;
    }
  }
}

void refreshRow()
{
  for (int row = 0; row < 8; ++row)
  {
    digitalWrite(rows[row], HIGH);
  }
}

void refreshCol()
{
  for (int col = 0; col < 8; ++col)
  {
    digitalWrite(cols[col], LOW);
  }
}




