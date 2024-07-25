// synacor challenge (jack@pngu.org)
#include "vm.h"

Stack s;
Memory m;

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <challenge.bin>\n", argv[0]);
    return EXIT_FAILURE;
  }
  stack_init(&s);
  mem_init(&m);
  int res = parse(argv[1]);
  mem_free(&m);
  stack_free(&s);
  return res;
}
