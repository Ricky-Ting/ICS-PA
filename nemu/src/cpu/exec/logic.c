#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
	rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
	operand_write(id_dest,&id_dest->val);
  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
	rtl_xor(&(id_dest->val),&(id_dest->val),&(id_src->val));
	operand_write(id_dest,&id_dest->val);
//only register to register, need fixing
	//CF<-0, OF-<0
	uint32_t flag=0;
	rtl_set_CF(&flag);
	rtl_set_OF(&flag);

  print_asm_template2(xor);
}

make_EHelper(or) {
  TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
