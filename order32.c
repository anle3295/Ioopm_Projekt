#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "order32.h"


void WhichEndian() {
  if(O32_HOST_ORDER == O32_LITTLE_ENDIAN) {
    puts("System is little-endian");
  }
  else
    puts("System is big-endian");
}

void main() {
  WhichEndian();
  printf("%d\n", __BIG_ENDIAN);
  printf("%d\n", __LITTLE_ENDIAN);
}