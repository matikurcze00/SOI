#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

#include "monitor.h"
#include "fifo.h"
using namespace std;
//semafory
Monitor global();

//klasa buffer
class Buffer
{
  private:
  Fifo data;
  
  public:
  Buffer()
  {
    srand( (unsigned)time(NULL));
  }
  bool canProcEven()
  {
    return data.even()<10? true : false;
      
  }
  bool canProcOdd()
  {
    return data.even()>data.odd()? true : false;     
  }
  bool canConsEven()
  {
    return data.even()>=3? true : false;
  }
  bool canConsOdd()
  {
    return data.odd()>=7? true: false;
  }
  void addEven()
  {

    data.insert((rand()%100)*2);
    cout<<("ProcAddEven done \n");
    printf("FIFO --------------- Odd: %d, Even: %d \n", data.odd(), data.even());
    
  } 
  void addOdd()
  {
    data.insert((rand()%100)*2-1);
    cout<<("ProcAddOdd done \n");
    printf("FIFO --------------- Odd: %d, Even: %d \n", data.odd(), data.even());
  }
  void popEven()
  {
    if(data.even()>0)
    {
      bool fEven = false;
      int temp;
      while(!fEven)
      {
      temp = data.pop();
      if(temp%2==0)
        fEven=true;
      else
        data.insert(temp);
      }
    }
    cout<<("ConsPopEven done \n");
    printf("FIFO --------------- Odd: %d, Even: %d \n", data.odd(), data.even());
  }
  void popOdd()
  {
    if(data.odd()>0)
    {
      bool fOdd = false;
      int temp;
      while(!fOdd)
      {
      temp = data.pop();
      if(temp%2==1)
        fOdd=true;
      else
        data.insert(temp);
      }
    }
    cout<<("ConsPopOdd done \n");
    printf("FIFO --------------- Odd: %d, Even: %d \n", data.odd(), data.even());
  }
};
