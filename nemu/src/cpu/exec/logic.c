#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&id_dest->val, &id_dest->val, &id_src->val);

  //printf("test:\tid_dest->val:%d\tid_src->val:%d\n", id_dest->val, id_src->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  rtlreg_t term = 0;
  rtl_set_CF(&term);
  rtl_set_OF(&term);

  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
  //printf("and:\tid_dest->val:%d\tid_src->val:%d\t", id_dest->val, id_src->val);

  rtl_and(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);

  //printf("id_dest->val:%d\n", id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  rtlreg_t term = 0;
  rtl_set_CF(&term);
  rtl_set_OF(&term);

  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
  //printf("xor:\tid_dest->val:%d\tid_src->val:%d\t", id_dest->val, id_src->val);
  rtl_xor(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);
  //printf("id_dest->val:%d\n", id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  rtlreg_t term = 0;
  rtl_set_CF(&term);
  rtl_set_OF(&term);

  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
  //printf("or:\tid_dest->val:%d\tid_src->val:%d\n", id_dest->val, id_src->val);
  rtl_or(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);
  //printf("or:\tid_dest->val:%d\n", id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);
  rtlreg_t term = 0;
  rtl_set_CF(&term);
  rtl_set_OF(&term);

  print_asm_template2(or);
}

make_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  //id_dest->val = (uint32_t)((int)id_dest->val >> id_src->val);
  //printf("sar:\tid_dest->val:%d\tid_src->val:%d\n", id_dest->val, id_src->val);
  /*
  t0 = id_dest->val & (1<<(id_dest->width*4-1));
  t1 = 0;
  if(t0)
    t1 = t0 | (t0-1);
  t0 = 0xffffffff;
  t2 = (t0>>(id_dest->width*4-id_src->val))<<(id_dest->width*4-id_src->val);
  */
  if(id_dest->width == 4)
    rtl_sar(&id_dest->val, &id_dest->val, &id_src->val);
  else if(id_dest->width == 2){
    rtl_sext(&t0, &id_dest->val, id_dest->width);
    rtl_sar(&t0, &t0, &id_src->val);
    id_dest->val = (id_dest->val & 0xffff0000) | (t0 & 0xffff);
  }
  else if(id_dest->width == 1){
    rtl_sext(&t0, &id_dest->val, id_dest->width);
    rtl_sar(&t0, &t0, &id_src->val);
    id_dest->val = (id_dest->val & 0xffffff00) | (t0 & 0xff);
  }
  //id_dest->val |= (t1&t2);
  //printf("sar:\tid_dest->val:%d\n", id_dest->val);
  operand_write(id_dest, &id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  //id_dest->val = (uint32_t)((unsigned int)id_dest->val << id_src->val);
  //printf("shl:\tid_dest->val:%d\tid_src->val:%d\n", id_dest->val, id_src->val);
  rtl_shl(&id_dest->val, &id_dest->val, &id_src->val);
  //printf("shl:\tid_dest->val:%d\n", id_dest->val);
  operand_write(id_dest, &id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&id_dest->val, &id_dest->val, &id_src->val);
  operand_write(id_dest, &id_dest->val);

  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //TODO();
  rtl_not(&id_dest->val, &id_dest->val);
  operand_write(id_dest, &id_dest->val);
    
  print_asm_template1(not);
}

make_EHelper(rcl) {
  //printf("rcl:%#x\t%d\t%x\t", id_dest->val, id_dest->width, id_src->val);
  t0 = id_src->val;
  t1 = id_dest->val;
  uint32_t term;
  rtl_get_CF(&term);
  t2 = 0xffffffff;
  uint32_t ori = (t2 >> (id_dest->width*8)) << (id_dest->width*8);
  //printf("ori:%#x\t", ori);
  uint32_t cha = ~ori;
  while(t0 != 0){
    t2 = term;
    rtl_msb(&term, &t1, id_dest->width);
    t1 = t1*2 + t2;
    t0--;
  }
  t1 = (id_dest->val & ori) | (t1 & cha);
  //printf("answer:%#x\n", t1);
  operand_write(id_dest, &t1);
  rtl_msb(&t0, &t1, id_dest->width);
  rtl_get_CF(&t2);
  t0 = term;
  if(t2 != t0){
    t0 = 1;
    rtl_set_OF(&t0);
  }
  else{
    t0 = 0;
    rtl_set_OF(&t0);
  }
  rtl_set_CF(&term);
  
  print_asm_template2(rcl);
}

make_EHelper(rcr) {
  //printf("rcr:%#x\t%d\t%x\t", id_dest->val, id_dest->width, id_src->val);
  t0 = id_src->val;
  t1 = id_dest->val;
  uint32_t term;
  rtl_get_CF(&term);
  t2 = 1;
  uint32_t ori = (t2 << ((id_dest->width*8)%32))-1;
  uint32_t cha = ~ori;
  while(t0 != 0){
    t2 = term;
    term = t1 & 1;
    t1 = t1/2 + t2 * (1<<(id_dest->width*8));
    t0--;
  }
  t1 = (id_dest->val & ori) | (t1 & cha);
  //printf("answer:%#x\n", t1);
  operand_write(id_dest, &t1);
  rtl_msb(&t0, &t1, id_dest->width);
  t2 = term;
  if(t2 != t0){
    t0 = 1;
    rtl_set_OF(&t0);
  }
  else{
    t0 = 0;
    rtl_set_OF(&t0);
  }
  rtl_set_CF(&term);
  
  print_asm_template2(rcr);
}

make_EHelper(rol) {
  /*
  rtl_get_CF(&t1);
  t0 = 0 | ((t1<<(id_src->val+id_dest->width*8-1))%(id_dest->width*8));
  for(int i = 0;i < id_dest->width*8-1;i++){
    t0 |= ((id_dest->val & (1<<i))>>i)<<((id_src->val+i)%id_dest->width);
  }
  */
  //printf("rol:%#x\t%d\t%x\t", id_dest->val, id_dest->width, id_src->val);
  t0 = id_src->val;
  t1 = id_dest->val;
  //t2 = 0xffffffff;
  t2 = 1;
  //uint32_t ori = (t2 >> (id_dest->width*8)) << (id_dest->width*8);
  uint32_t ori = (t2 << ((id_dest->width*8)%32))-1;
  uint32_t cha = ~ori;
  //printf("ori:%#x\tcha:%#x\tie_dest->widt*8:%d\t", ori, cha, id_dest->width*8);
  while(t0 != 0){
    rtl_msb(&t2, &t1, id_dest->width);
    t1 = t1*2 + t2;
    t0--;
  }
  t1 = (id_dest->val & ori) | (t1 & cha);
  //printf("answer:%#x\n", t1);
  operand_write(id_dest, &t1);
  rtl_msb(&t0, &t1, id_dest->width);
  rtl_get_CF(&t2);
  if(t2 != t0){
    t0 = 1;
    rtl_set_OF(&t0);
  }
  else{
    t0 = 0;
    rtl_set_OF(&t0);
  }
  
  print_asm_template2(rol);
  /*
  t2 = 1;
  t2 = t2 << 16;
  printf("test:%#x\n", t2); 
  */
  //TODO();
}

make_EHelper(ror) {
  //printf("ror:%#x\t%d\t%x\t", id_dest->val, id_dest->width, id_src->val);
  t0 = id_src->val;
  t1 = id_dest->val;
  t2 = 1;
  uint32_t ori = (t2 << ((id_dest->width*8)%32))-1;
  uint32_t cha = ~ori;
  while(t0 != 0){
    t2 = t1 & 1;
    t1 = t1/2 + t2 * (1<<(id_dest->width*8));
    t0--;
  }
  t1 = (id_dest->val & ori) | (t1 & cha);
  //printf("answer:%#x\n", t1);
  operand_write(id_dest, &t1);
  rtl_msb(&t0, &t1, id_dest->width);
  t2 = id_dest->val & (1<<(id_dest->width*8-2));
  if(t2 != t0){
    t0 = 1;
    rtl_set_OF(&t0);
  }
  else{
    t0 = 0;
    rtl_set_OF(&t0);
  }
  
  print_asm_template2(ror);
}
