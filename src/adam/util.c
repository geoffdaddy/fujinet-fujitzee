#ifdef __ADAM__
#include <eos.h>
#include <time.h>

unsigned long currentTime;
/*
  Platform specific utilities that don't fit in any category
*/

void housekeeping() {};
uint8_t getJiffiesPerSecond() {return 60;};


void resetTimer()
{
  currentTime = 0;
}

int getTime()
{
  return currentTime;
}

void quit()
{
  eos_exit_to_smartwriter();
}


#endif