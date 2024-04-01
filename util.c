// synacor challenge (@waffle87)
#include "util.h"

#ifndef NDEBUG
char *reg_string(unsigned short r) {
  static char str[6];
  switch (r) {
  case 32768:
    sprintf(str, "r0");
    break;
  case 32769:
    sprintf(str, "r1");
    break;
  case 32770:
    sprintf(str, "r2");
    break;
  case 32771:
    sprintf(str, "r3");
    break;
  case 32772:
    sprintf(str, "r4");
    break;
  case 32773:
    sprintf(str, "r5");
    break;
  case 32774:
    sprintf(str, "r6");
    break;
  case 32775:
    sprintf(str, "r7");
    break;
  default:
    sprintf(str, "null");
  }
  return str;
}
#endif // NDEBUG
