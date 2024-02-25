#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void step(int current_step)
{
  printf("step %i executed\n", current_step);
}

void steps()
{
  int current_step = 0;

  switch(current_step)
  {
    case 0: step(current_step); current_step++;
    case 1: step(current_step); current_step++;
    case 2: step(current_step); current_step++;
    case 3: step(current_step);
    default: break;
  }
}

int main( int argc, char *argv[] )
{
  steps();
  exit( 0 );
}
