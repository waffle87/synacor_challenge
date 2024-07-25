// synacor challenge (jack@pngu.org)
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef int (*instruction_ptr)(u8 *buffer);

#ifdef NDEBUG
#define LOG(fmt, ...) ((void)0)
#define REG(x) ((void)0)
#else
#define LOG(...) fprintf(stdout, __VA_ARGS__)
#define REG(x) reg_string(x)
char *reg_string(unsigned short r);
#endif
