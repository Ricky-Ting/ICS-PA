#include "cpu/exec.h"

inline uint32_t get_s(uint32_t input, int width){
  return ((input>>(width-1))&1) == 1;
}

make_EHelper(add) {
  //TODO();
  /*
  uint64_t check = (uint64_t)(id_dest->val + id_src->val);
  uint32_t check_l = id_dest->val + id_src->val;
  uint32_t change1 = (check >= ((uint64_t)1<<32));
  rtl_set_CF(&change1);
  uint32_t change2 = ((g32b(check_l)&&g32b(id_dest->val)&&(!g32b(id_src->val))) || ((!g32b(check_l))&&(!g32b(id_dest->val))&&g32b(id_src->val)));  
  rtl_set_OF(&change2);
  */
  //printf("add:\tid_dest->val:%d\tid_src->val:%d\t", id_dest->val, id_src->val);
  t1 = id_dest->val;

  //int sa = get_s(id_dest->val, id_dest->width), sb = get_s(id_src->val, id_src->width);
  //uint64_t term = (uint64_t)id_dest->val + (uint64_t)id_src->val;

  rtl_add(&id_dest->val, &id_dest->val, &id_src->val);
  //int sr = get_s(id_dest->val, id_dest->width);
  operand_write(id_dest, &id_dest->val);

  //printf("id_dest->val:%d\n", id_dest->val);
  
  //uint32_t term1 = (term>>(id_dest->width*8))&1;
  //rtl_set_CF(&term1); //Here decide the mode of CF
  //uint32_t term2 = !(sa==sb&&sb==sr);
  //rtl_set_OF(&term2);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t1);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &t1, &id_src->val);
  rtl_not(&t0, &t0);
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  //printf("CF:%d\tOF:%d\tZF:%d\tSF:%d\n", cpu_CF(), cpu_OF(), cpu_ZF(), cpu_SF());

  print_asm_template2(add);

  //printf("add:\tid_dest:%d\n", id_dest->val);
  //printf("esi:%d\n", cpu.esi);
}

make_EHelper(sub) {
  //TODO();
  /*
  uint64_t check = (uint64_t)(id_dest->val - id_src->val);
  uint32_t check_l = id_dest->val - id_src->val;
  uint32_t change1 = (check >= ((uint64_t)1<<32));
  rtl_set_CF(&change1);
  uint32_t change2 = ((g32b(check_l)&&g32b(id_dest->val)&&(!g32b(id_src->val))) || ((!g32b(check_l))&&(!g32b(id_dest->val))&&g32b(id_src->val)));  
  rtl_set_OF(&change2);
  */
  //int sa = get_s(id_dest->val, id_dest->width), sb = -get_s(id_src->val, id_src->width);
  //uint64_t term = (uint64_t)id_dest->val - (uint64_t)id_src->val;
  t1 = id_dest->val;

  rtl_sub(&id_dest->val, &id_dest->val, &id_src->val);
  //int sr = get_s(id_dest->val, id_dest->width);
  operand_write(id_dest, &id_dest->val);

  /*
  uint32_t term1 = (term>>(id_dest->width*8))&1;
  rtl_set_CF(&term1); //Here decide the mode of CF
  uint32_t term2 = !(sa==sb&&sb==sr);
  rtl_set_OF(&term2);
  */
  rtl_setrelop(RELOP_LTU, &t0, &t1, &id_dest->val);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &t1, &id_src->val);
  rtl_xor(&t2, &id_dest->val, &t1);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  //printf("CF:%d\tOF:%d\tZF:%d\tSF:%d\n", cpu_CF(), cpu_OF(), cpu_ZF(), cpu_SF());
  //printf("sub:\tt1:%d\tid_dest->val:%d\tid_src->val:%d\n", t1, id_dest->val, id_src->val);

  print_asm_template2(sub);
}

make_EHelper(cmp) {
  //TODO();
  //printf("cmp\t id_dest:%d\tid_src:%d\n", id_dest->val, id_src->val);
  //printf("edi:%d\tebx:%d\tedx:%d\n", cpu.edi, cpu.ebx, cpu.edx);

  //int sa = get_s(id_dest->val, id_dest->width), sb = -get_s(id_src->val, id_src->width);
  //uint64_t term = (uint64_t)id_dest->val - (uint64_t)id_src->val;

  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  //printf("cmp:%#x\n", id_src->val);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  t1 = id_dest->val;
  rtl_sub(&id_dest->val, &id_dest->val, &id_src->val);
  /*
  int sr = get_s(id_dest->val, id_dest->width);

  uint32_t term1 = (term>>(id_dest->width*8))&1;
  rtl_set_CF(&term1); //Here decide the mode of CF
  uint32_t term2 = !(sa==sb&&sb==sr);
  rtl_set_OF(&term2);
  */

  rtl_setrelop(RELOP_LTU, &t0, &t1, &id_dest->val);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &t1, &id_src->val);
  rtl_xor(&t2, &id_dest->val, &t1);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  //printf("cmp:CF:%d\tOF:%d\tZF:%d\tSF:%d\tresult:%d\n", cpu_CF(), cpu_OF(), cpu_ZF(), cpu_SF(), id_dest->val);

  //printf("cmp:\tt1:%d\tid_dest->val:%d\tid_src->val:%d\n", t1, id_dest->val, id_src->val);
  

  print_asm_template2(cmp);
}

make_EHelper(inc) {
  //TODO();
  t1 = id_dest->val;
  id_src->val = 1;
  
  id_dest->val++;
  operand_write(id_dest, &id_dest->val);

  //printf("dec:\tid_dest->val:%d\n", id_dest->val);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t1);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &t1, &id_src->val);
  rtl_not(&t0, &t0);
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  //printf("inc:\tt1:%d\tid_dest->val:%d\n", t1, id_dest->val);
  
  print_asm_template1(inc);
}

make_EHelper(dec) {
  //TODO();
  //uint32_t term = id_dest->val;
  t1 = id_dest->val;
  id_src->val = 1;
  
  id_dest->val--;
  operand_write(id_dest, &id_dest->val);

  //printf("dec:\tid_dest->val:%d\n", id_dest->val);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t1);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &t1, &id_src->val);
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  
  //uint32_t set_OF = (rtl_msb(id_dest) == 0 && rtl_msb(&term) == 1)&1;
  //rtl_set_OF(&set_OF);

  print_asm_template1(dec);
}

make_EHelper(neg) {
  //TODO();
  t0 = 0;
  t1 = 1;
  if(id_dest->val == 0)
    rtl_set_CF(&t0);
  else
    rtl_set_CF(&t1);
  if(((id_dest->val & ((1<<(id_dest->width))-1)) ^ (1<<(id_dest->width-1))) == 0)
    rtl_set_OF(&t1);
  else
    rtl_set_OF(&t0);
  id_dest->val = -id_dest->val;
  operand_write(id_dest, &id_dest->val);
  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  print_asm_template1(neg);
}

make_EHelper(adc) {
  //printf("adc:\tid_dest->val:%d\tid_src->val:%d\t", id_dest->val, id_src->val);

  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &t2, &id_dest->val);
  rtl_get_CF(&t1);
  rtl_add(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  //printf("t2:%d\n", t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &t2, &id_dest->val);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0, &t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  //printf("sbb:\tid_dest->val:%d\tid_src->val:%d\t", id_dest->val, id_src->val);

  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  //printf("t2:%d\n", t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_mul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_imul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template2(imul);

  //printf("imul2:\tid_dest->val:%d\tid_src->val:%d\tt2:%d\n", id_dest->val, id_src->val, t2);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_div64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_div64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(div);

  //printf("div:\tcpu.eax:%d\tcpu.edx:%d\n", cpu.eax, cpu.edx);
}

make_EHelper(idiv) {
  //printf("idiv:\tcpu.eax:%d\tid_dest->val:%d\t", cpu.eax, id_dest->val);

  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_idiv64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_idiv64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(idiv);

  //printf("cpu.eax:%d\tcpu.edx:%d\n", cpu.eax, cpu.edx);
}
