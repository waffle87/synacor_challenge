// synacor challenge (jack@pngu.org)
#include "memory.h"
#include "stack.h"
#include "util.h"

extern Stack s;
extern Memory m;

int parse(char const *filename);
int operate(u16 opcode, u8 *buffer);

int is_valid(u16 opcode);
int is_literal(u16 opcode);
int is_register(u16 opcode);
u16 *get_register(u16 opcode);
instruction_ptr get_instruction(u16 opcode);

int instruction_halt(u8 *buffer);
int instruction_set(u8 *buffer);
int instruction_push(u8 *buffer);
int instruction_pop(u8 *buffer);
int instruction_eq(u8 *buffer);
int instruction_gt(u8 *buffer);
int instruction_jmp(u8 *buffer);
int instruction_jt(u8 *buffer);
int instruction_jf(u8 *buffer);
int instruction_add(u8 *buffer);
int instruction_mult(u8 *buffer);
int instruction_mod(u8 *buffer);
int instruction_and(u8 *buffer);
int instruction_or(u8 *buffer);
int instruction_not(u8 *buffer);
int instruction_rmem(u8 *buffer);
int instruction_wmem(u8 *buffer);
int instruction_call(u8 *buffer);
int instruction_ret(u8 *buffer);
int instruction_out(u8 *buffer);
int instruction_in(u8 *buffer);
int instruction_noop(u8 *buffer);
