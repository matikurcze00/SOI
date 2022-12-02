#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#ifdef _WIN32
    #include <windows.h>

    void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>
    
    void sleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif
#include "monitor.h"
#include "fifo.h"
//semafory
Semaphore global(1), semProdEven(0),semProdOdd(0), semConsEven(0), semConsOdd(0);
//zmienne globalne
unsigned int numOfProdEvenWaiting = 0, numOfProdOddWaiting = 0, numOfConsEvenWaiting = 0, numOfConsOddWaiting = 0;
int ConsTime = 2000;
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
    global.p();
    if(!canProcEven())
    {
      ++numOfProdEvenWaiting;
      global.v();
      semProdEven.p();
      --numOfProdEvenWaiting;
    }
    data.insert((rand()%100)*2);
    if(numOfProdOddWaiting > 0 && canProcOdd())
      semProdOdd.v();
    else if (numOfConsEvenWaiting > 0 && canConsEven())
      semConsEven.v();
    else if (numOfConsOddWaiting > 0 && canConsOdd())
      semConsOdd.v();
    else
      global.v();
  } 
  void addOdd()
  {
    global.p();
    if(!canProcOdd())
    {
      ++numOfProdOddWaiting;
      global.v();
      semProdOdd.p();
      --numOfProdOddWaiting;
    }
    data.insert((rand()%100)*2-1);
    if (numOfConsEvenWaiting > 0 && canConsEven())
      semConsEven.v();
    else if (numOfConsOddWaiting > 0 && canConsOdd())
      semConsOdd.v();
    else if (numOfProdEvenWaiting > 0 && canProcOdd())
      semProdEven.v() ;
    else
      global.v();
  }
  void popEven()
  {
    global.p();
    if(!canConsEven())
    {
      ++numOfConsEvenWaiting;
      global.v();
      semConsEven.p();
      --numOfConsEvenWaiting;
    }
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
    if (numOfConsOddWaiting > 0 && canConsOdd())
      semConsOdd.v();
    else if (numOfProdEvenWaiting > 0 && canProcOdd())
      semProdEven.v() ;
    else if (numOfProdOddWaiting > 0 && canProcOdd())
      semProdOdd.v();
    else
      global.v();
  }
  void popOdd()
  {
        global.p();
    if(!canConsOdd())
    {
      ++numOfConsOddWaiting;
      global.v();
      semConsOdd.p();
      --numOfConsOddWaiting;
    }
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
    if (numOfProdEvenWaiting > 0 && canProcOdd())
      semProdEven.v() ;
    else if (numOfProdOddWaiting > 0 && canProcOdd())
      semProdOdd.v();
    else if (numOfConsEvenWaiting > 0 && canConsEven())
      semConsEven.v();
    else
      global.v();
  }
};
Buffer buffer;
//procesy
void* prodEven(void *)
{
  while (true)
  {
    buffer.addEven();
    sleep(rand()%2000+ConsTime);
  }
}
void* prodOdd(void *)
{
  while (true)
  {
    buffer.addOdd();
    sleep(rand()%2000+ConsTime);
  }
}
void* consEven(void *)
{
  while (true)
  {
    buffer.popEven();
    sleep(rand()%2000+ConsTime);
  }
}
void* consOdd(void *)
{
  while (true)
  {
    buffer.popOdd();
    sleep(rand()%2000+ConsTime);
  }
}
int main(){
  pthread_t prodOddTh, prodEvenTh, consOddTh, consEvenTh;

  pthread_create(&prodOddTh, NULL, &prodOdd, NULL);
  pthread_create(&prodEvenTh, NULL, &prodEven, NULL);
  pthread_create(&consOddTh, NULL, &consOdd, NULL);
  pthread_create(&consEvenTh, NULL, &consEven, NULL);

  pthread_join(prodOddTh, NULL);
  pthread_join(prodEvenTh, NULL);
  pthread_join(consOddTh, NULL);
  pthread_join(consEvenTh, NULL);
}