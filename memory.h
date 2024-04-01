// synacor challenge (@waffle87)
#include "util.h"

typedef struct {
  u8 *mem;
  u16 size;
} Memory;

void mem_init(Memory *m);
void mem_free(Memory *m);
void mem_set(Memory *m, u16 addr, u16 value);
u16 mem_get(Memory *m, u16 addr);
u8 *get_next_pair(u8 *buffer, u16 *pair);
