// synacor challenge (jack@pngu.org)
#include "util.h"

typedef struct {
  u16 *data;
  u16 pointer;
} Stack;

void stack_init(Stack *s);
void stack_free(Stack *s);
void stack_push(Stack *s, u16 value);
u16 stack_pop(Stack *s);
