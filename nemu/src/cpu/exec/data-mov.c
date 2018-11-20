#include "cpu/exec.h"

make_EHelper(mov) {
  //printf("mov:\tid_src->val:%d\tid_dest->val:%d\n", id_src->val, id_dest->val);
  //printf("cpu.esi:%#x\n", cpu.esi);

  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
  //printf("push\tval:%d\n", id_dest->val);
  /*
  if(id_dest->width == 2){
    cpu.esp -= 2;
    vaddr_write(cpu.esp, id_dest->val, 2);
  }
  else
    */
  rtl_push(&id_dest->val);
  //printf("hahaha\n");
  //printf("push:%#x\n", id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
  //TODO();
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);
  //printf("fff\n");
  //printf("pop:%#x\n", id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  //TODO();
  //printf("pusha:\neax:%#x\tecx:%#x\tedx:%#x\tebx:%#x\nesp:%#x\tebp:%#x\tesi:%#x\tedi:%#x\n", cpu.eax, cpu.ecx, cpu.edx, cpu.ebx, cpu.esp, cpu.ebp, cpu.esi, cpu.edi);
  t0 = cpu.esp;
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&t0);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);

  print_asm("pusha");
}

make_EHelper(popa) {
  //TODO();
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&t0);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  cpu.esp = cpu.ebp;
  rtl_pop(&cpu.ebp);

  print_asm("leave");
}

make_EHelper(cltd) {
  //printf("cltd:\tcpu.eax:%#x\t", cpu.eax);
  if (decoding.is_operand_size_16) {
    //TODO();
    /*
    rtl_msb(&t0, &cpu.eax, 2);
    cpu.edx = t0<<31;
    rtl_sext(&cpu.eax, &cpu.eax, 2);
    */
    if((int32_t)reg_w(R_AX) < 0)
      reg_w(R_DX) = 0xffff;
     else
      reg_w(R_DX) = 0;
  }
  else {
    //TODO();
    /*
    rtl_msb(&t0, &cpu.eax, 2);
    cpu.edx = t0<<31;
    */
    if((int32_t)reg_l(R_EAX) < 0)
      reg_l(R_EDX) = 0xffffffff;
    else
      reg_l(R_EDX) = 0;
  }

  //printf("cpu.eax:%#x\tcpu.edx:%#x\n", cpu.eax, cpu.edx);

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    //TODO();
    t1 = 2;
    rtl_sext(&t0, &reg_l(R_AX), t1);
    reg_l(R_AX) = (t0 & 0xffff) | reg_l(R_AX);
  }
  else {
    //TODO();
    t1 = 4;
    rtl_sext(&t0, &reg_l(R_AX), t1);
    reg_l(R_AX) = t0;
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);

  //printf("movzx\tid_dest:%d\tid_src:%d\n", id_dest->val, id_src->val);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
