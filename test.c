#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main()
{
  for (int i = 0; i <= 190; i++)
  {
    printf("\033[%dm%s", i, "hello\n");
  }
  printf("\033[0m");
  return 0;
};
