#include "monitor.h"
#include "buffer.h"
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
unsigned int numOfProdEvenWaiting = 0, numOfProdOddWaiting = 0, numOfConsEvenWaiting = 0, numOfConsOddWaiting = 0;
//zmienne globalne
int ConsTime = 2000;
using namespace std;

class MyMonitor : public Monitor
{
  Condition condProdOdd, condProdEven, condConsOdd, condConsEven;
  Buffer buffer;
  public:
  MyMonitor() {
    Monitor();

    }
   void addEven()
  {
    enter();
    if(!buffer.canProcEven())
    {
      ++numOfProdEvenWaiting;
      cout<<("ProcAddEven is waiting \n");
      wait(condProdEven);
      --numOfProdEvenWaiting;
    }
    buffer.addEven();
    if(numOfProdOddWaiting > 0 && buffer.canProcOdd())
      condProdOdd.signal();
    else if (numOfConsEvenWaiting > 0 && buffer.canConsEven())
      condConsEven.signal();
    else if (numOfConsOddWaiting > 0 && buffer.canConsOdd())
      condConsOdd.signal();
    else
      leave();
  }

  void addOdd()
  {
    enter();
    if(!buffer.canProcOdd())
    {
      ++numOfProdOddWaiting;
      cout<<("ProcAddOdd is waitnig \n");
      wait(condProdOdd);
      --numOfProdOddWaiting;
    }
    buffer.addOdd();

    if(numOfConsEvenWaiting > 0 && buffer.canConsEven())
      condConsEven.signal();
    else if (numOfConsOddWaiting > 0 && buffer.canConsOdd())
      condConsOdd.signal();
    else if (numOfProdEvenWaiting > 0 && buffer.canProcOdd())
      condProdEven.signal() ;
    else
      leave();
  }

  void popEven()
  {
    enter();
    if(!buffer.canConsEven())
    {
      ++numOfConsEvenWaiting;
      cout<<("ConsPopEven is waiting \n");
      wait(condConsEven);
      --numOfConsEvenWaiting;
    }
    buffer.popEven();
    if (numOfConsOddWaiting > 0 && buffer.canConsOdd())
      condConsOdd.signal();
    else if (numOfProdEvenWaiting > 0 && buffer.canProcOdd())
      condProdEven.signal() ;
    else if (numOfProdOddWaiting > 0 && buffer.canProcOdd())
      condProdOdd.signal();
    else
      leave();
  }

  void popOdd()
  {
    enter();
    if(!buffer.canConsOdd())
    {
      ++numOfConsOddWaiting;
      cout<<("ConsPopOdd is waiting \n");

      wait(condConsOdd);
      --numOfConsOddWaiting;
    }
    buffer.popOdd();
    if (numOfProdEvenWaiting > 0 && buffer.canProcOdd())
      condProdEven.signal() ;
    else if (numOfProdOddWaiting > 0 && buffer.canProcOdd())
      condProdOdd.signal();
    else if (numOfConsEvenWaiting > 0 && buffer.canConsEven())
      condConsEven.signal();
    else
      leave();
  }
};

MyMonitor monitor;

void* prodEven(void *)
{
  while (true)
  {
    monitor.addEven();
    sleep(rand()%2000+ConsTime);
  }
}
void* prodOdd(void *)
{
  while (true)
  {
    monitor.addOdd();
    sleep(rand()%2000+ConsTime);
  }
}
void* consEven(void *)
{
  while (true)
  {
    monitor.popEven();
    sleep(rand()%3000+ConsTime);
  }
}
void* consOdd(void *)
{
  while (true)
  {
    monitor.popOdd();
    sleep(rand()%3000+ConsTime);
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