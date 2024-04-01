// synacor challenge (@waffle87)
#include "vm.h"

u16 rPC = 0;
u16 r0 = 0;
u16 r1 = 0;
u16 r2 = 0;
u16 r3 = 0;
u16 r4 = 0;
u16 r5 = 0;
u16 r6 = 0;
u16 r7 = 0;

int parse(char const *filename) {
  FILE *fptr = fopen(filename, "rb");
  if (!fptr) {
    fprintf(stderr, "error: opening file: %s\n", filename);
    return EXIT_FAILURE;
  }
  if (fseek(fptr, 0, SEEK_END)) {
    fprintf(stderr, "error: opening file: %s\n", filename);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  long file_size = ftell(fptr);
  if (file_size == EOF) {
    fprintf(stderr, "error: opening file: %s\n", filename);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  if (fseek(fptr, 0, SEEK_SET)) {
    fprintf(stderr, "error: opening file: %s\n", filename);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  size_t size_read = fread(m.mem, 1, file_size, fptr);
  if (size_read != file_size) {
    fprintf(stderr, "error: reading file: %s\n", filename);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  int res;
  u16 opcode;
  u8 *buffer = m.mem;
  rPC = 0;
  while (1) {
    get_next_pair(buffer + rPC, &opcode);
    res = operate(opcode, buffer + rPC + 2);
    if (res == -1)
      break;
    rPC = res;
  }
  fclose(fptr);
  return EXIT_SUCCESS;
}

int operate(u16 opcode, unsigned char *buffer) {
  instruction_ptr instr;
  int next;
  instr = get_instruction(opcode);
  if (!instr)
    LOG("opcode: %d\n", opcode);
  assert(instr);
  next = instr(buffer);
  if (next == -1)
    return EXIT_FAILURE;
  return next;
}

int is_valid(u16 opcode) { return opcode < 32776; }

int is_literal(u16 opcode) { return opcode < 32768; }

int is_register(u16 opcode) { return opcode >= 32768 && opcode <= 32775; }

u16 *get_register(u16 opcode) {
  switch (opcode) {
  case 32768:
    return &r0;
  case 32769:
    return &r1;
  case 32770:
    return &r2;
  case 32771:
    return &r3;
  case 32772:
    return &r4;
  case 32773:
    return &r5;
  case 32774:
    return &r6;
  case 32775:
    return &r7;
  default:
    return NULL;
  }
}

instruction_ptr get_instruction(u16 opcode) {
  switch (opcode) {
  case 0:
    return &instruction_halt;
  case 1:
    return &instruction_set;
  case 2:
    return &instruction_push;
  case 3:
    return &instruction_pop;
  case 4:
    return &instruction_eq;
  case 5:
    return &instruction_gt;
  case 6:
    return &instruction_jmp;
  case 7:
    return &instruction_jt;
  case 8:
    return &instruction_jf;
  case 9:
    return &instruction_add;
  case 10:
    return &instruction_mult;
  case 11:
    return &instruction_mod;
  case 12:
    return &instruction_and;
  case 13:
    return &instruction_or;
  case 14:
    return &instruction_not;
  case 15:
    return &instruction_rmem;
  case 16:
    return &instruction_wmem;
  case 17:
    return &instruction_call;
  case 18:
    return &instruction_ret;
  case 19:
    return &instruction_out;
  case 20:
    return &instruction_in;
  case 21:
    return &instruction_noop;
  default:
    return NULL;
  }
}

int instruction_halt(u8 *buffer) { return -1; }

int instruction_set(u8 *buffer) {
  u16 next, *a;
  buffer = get_next_pair(buffer, &next);
  LOG("set: %d", next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &next);
  if (is_register(next))
    next = *get_register(next);
  LOG(" %d\n", next);
  *a = next;
  return rPC + 6;
}

int instruction_push(u8 *buffer) {
  u16 next;
  buffer = get_next_pair(buffer, &next);
  if (is_register(next))
    next = *get_register(next);
  stack_push(&s, next);
  LOG("push: %d\n", next);
  return rPC + 4;
}

int instruction_pop(u8 *buffer) {
  u16 next, *a;
  buffer = get_next_pair(buffer, &next);
  a = get_register(next);
  *a = stack_pop(&s);
  LOG("pop: %d\n", *a);
  return rPC + 4;
}

int instruction_eq(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  LOG("%d: eq: %s", rPC, REG(next));
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  LOG(" %s", REG(b));
  if (is_register(b))
    b = *get_register(b);
  LOG("(%d)", b);
  buffer = get_next_pair(buffer, &c);
  LOG(" %s", REG(c));
  if (is_register(c))
    c = *get_register(c);
  LOG("(%d)\n", c);
  *a = b == c ? 1 : 0;
  return rPC + 8;
}

int instruction_gt(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  LOG("gt: %d", next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  *a = b > c ? 1 : 0;
  LOG(" %d %d\n", b, c);
  return rPC + 2 + 6;
}

int instruction_jmp(u8 *buffer) {
  u16 next;
  buffer = get_next_pair(buffer, &next);
  if (is_register(next))
    next = *get_register(next);
  LOG("jmp: %d\n", next);
  return next * 2;
}

int instruction_jt(u8 *buffer) {
  u16 a, b;
  buffer = get_next_pair(buffer, &a);
  LOG("jt: %d", a);
  if (is_register(a))
    a = *get_register(a);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  LOG(" %d\n", b);
  return a ? b * 2 : rPC + 6;
}

int instruction_jf(u8 *buffer) {
  u16 a, b;
  buffer = get_next_pair(buffer, &a);
  LOG("jf: %d", a);
  if (is_register(a))
    a = *get_register(a);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  LOG(" %d\n", b);
  return !a ? b * 2 : rPC + 6;
}

int instruction_add(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  a = get_register(next);
  LOG("add: %d ", next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  *a = (b + c) % 32768;
  LOG("%d %d\n", b, c);
  return rPC + 8;
}

int instruction_mult(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  a = get_register(next);
  LOG("mult: %d ", next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  *a = (b * c) % 32768;
  LOG("%d %d\n", b, c);
  return rPC + 8;
}

int instruction_mod(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  a = get_register(next);
  LOG("mod: %d ", next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  *a = (b % c);
  LOG("%d %d\n", b, c);
  return rPC + 8;
}

int instruction_and(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  LOG("and: %d", next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  LOG(" %d %d\n", b, c);
  *a = b & c;
  return rPC + 8;
}

int instruction_or(u8 *buffer) {
  u16 next, *a, b, c;
  buffer = get_next_pair(buffer, &next);
  LOG("or: %d", next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  buffer = get_next_pair(buffer, &c);
  if (is_register(c))
    c = *get_register(c);
  LOG(" %d %d\n", b, c);
  *a = b | c;
  return rPC + 8;
}

int instruction_not(u8 *buffer) {
  u16 next, *a, b;
  buffer = get_next_pair(buffer, &next);
  LOG("not: %d", next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  if (is_register(b))
    b = *get_register(b);
  LOG(" %d\n", b);
  *a = ~b;
  *a ^= 0x8000;
  return rPC + 6;
}

int instruction_rmem(u8 *buffer) {
  u16 next, *a, b;
  buffer = get_next_pair(buffer, &next);
  LOG("%d: rmem: %d", rPC, next);
  a = get_register(next);
  buffer = get_next_pair(buffer, &b);
  LOG(" %d\n", b);
  if (is_register(b))
    b = *get_register(b);
  *a = mem_get(&m, b);
  return rPC + 6;
}

int instruction_wmem(u8 *buffer) {
  u16 a, b;
  buffer = get_next_pair(buffer, &a);
  LOG("wmem: %d", a);
  if (is_register(a))
    a = *get_register(a);
  buffer = get_next_pair(buffer, &b);
  LOG(" %d\n", b);
  if (is_register(b))
    b = *get_register(b);
  mem_set(&m, a, b);
  return rPC + 6;
}

int instruction_call(u8 *buffer) {
  u16 a;
  buffer = get_next_pair(buffer, &a);
  if (is_register(a))
    a = *get_register(a);
  LOG("call: %d\n", a);
  stack_push(&s, (rPC + 4) / 2);
  return a * 2;
}

int instruction_ret(u8 *buffer) {
  u16 addr;
  LOG("ret\n");
  addr = stack_pop(&s);
  return addr * 2;
}

int instruction_out(u8 *buffer) {
  u16 next;
  buffer = get_next_pair(buffer, &next);
  if (is_register(next))
    next = *get_register(next);
  fprintf(stdout, "%c", next);
  return rPC + 4;
}

int instruction_in(u8 *buffer) {
  char in;
  u16 addr, *reg = NULL;
  get_next_pair(buffer, &addr);
  reg = get_register(addr);
  LOG("in: %d\n", addr);
  in = getchar();
  *reg = in;
  return rPC + 4;
}

int instruction_noop(u8 *buffer) { return rPC + 2; }
