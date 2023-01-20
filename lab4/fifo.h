#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

class Fifo
{
  private:
  int queue[20];
  int length = 0;
  int oddNumbers=0;
  int evenNumbers=0;
  
  public:
  void insert(int value)
  {
    queue[length] = value;
    ++length;
    value%2==0? ++evenNumbers: ++oddNumbers;
  }
  int pop()
  {
    try {
      if(length>0)
      {
        int temp = queue[0];
        temp%2==0? --evenNumbers: --oddNumbers;
        for(int i = 0; i<length-1 ;i++)
        {
          queue[i] = queue[i+1];
        }
        --length; 
        return temp;
      }
      else
      {
        throw std::invalid_argument("FIFO is empty");
      }
    }
    catch(std::invalid_argument& ex)
    {
      std::cerr << ex.what(); 
      return 0;
    }
  }
  int odd()
  {
    return oddNumbers;
  }
  int even()
  {
    return evenNumbers;
  }

};