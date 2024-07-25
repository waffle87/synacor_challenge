// synacor challenge (jack@pngu.org)
#include "memory.h"

void mem_init(Memory *m) {
  m->size = INT16_MAX;
  m->mem = malloc(sizeof(u16) * m->size);
  memset(m->mem, 0, sizeof(u16) * m->size);
}

void mem_free(Memory *m) {
  free(m->mem);
  m->size = 0;
}

void mem_set(Memory *m, u16 addr, u16 value) {
  addr = addr * 2;
  m->mem[addr] = value;
  m->mem[addr + 1] = value >> 8;
}

u16 mem_get(Memory *m, u16 addr) {
  u16 pair;
  addr = addr * 2;
  get_next_pair(m->mem + addr, &pair);
  return pair;
}

u8 *get_next_pair(u8 *buffer, u16 *pair) {
  *pair = (*(buffer + 1) << 8) + *buffer;
  return buffer + 2;
}
