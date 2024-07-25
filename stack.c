// synacor challenge (jack@pngu.org)
#include "stack.h"

void stack_init(Stack *s) {
  s->data = malloc(UINT16_MAX);
  s->pointer = 0;
}

void stack_free(Stack *s) { free(s->data); }

void stack_push(Stack *s, u16 value) {
  s->data[s->pointer] = value;
  s->pointer = s->pointer + 1;
}

u16 stack_pop(Stack *s) {
  u16 value = s->data[s->pointer - 1];
  s->pointer = s->pointer - 1;
  return value;
}
