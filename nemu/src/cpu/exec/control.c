#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);

  //printf("CF:%d\tOF:%d\tZF:%d\tSF:%d\n", (int)cpu_CF(), (int)cpu_OF(), (int)cpu_ZF(), (int)cpu_SF());

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  //TODO();
  //printf("hhh\n");
  //uint32_t term = cpu.eip + id_dest->width + 1;
  //printf("call:cpu.eip:%#x\n", cpu.eip);
  //rtl_push(&term);
  //printf("ppp\n");
  //rtl_add(&decoding.jmp_eip, &cpu.eip, &id_dest->imm);
  //decoding.jmp_eip = cpu.eip + id_dest->imm + id_dest->width + 1;
  //printf("qqq\n");
  //printf("decoding.jmp_eip:%#x\tcpu.eip:%#x\tid_dest->imm:%#x\n", decoding.jmp_eip, cpu.eip, id_dest->imm);
  //printf("width:%d\n", id_dest->width);
  rtl_push(eip);
  //printf("call:\tdecoding.jmp_eip:%#x\n", decoding.jmp_eip);
  rtl_j(decoding.jmp_eip);

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  //TODO();
  rtl_pop(&decoding.jmp_eip);
  rtl_j(decoding.jmp_eip);
  //printf("decoding.jmp_eip:%#x\n", decoding.jmp_eip);

  print_asm("ret");
}

make_EHelper(call_rm) {
  //TODO();
  /*
  rtl_lm(&decoding.jmp_eip, &id_dest->imm, id_dest->width);
  rtl_push(eip);
  rtl_j(decoding.jmp_eip);
  */
  rtl_push(eip);
  rtl_jr(&id_dest->val);

  print_asm("call *%s", id_dest->str);
}
