#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  //TODO();
	rtl_and(&id_dest->val,&id_dest->val,&id_src->val);

	uint32_t flag=0;
//	printf("Eflag:%#x\n",cpu.eflags); //debug
	rtl_set_CF(&flag);
	rtl_set_OF(&flag);
	rtl_update_ZFSF(&id_dest->val,id_dest->width);
//	printf("res=%#x\n",id_dest->val); //debug
//	printf("Eflag:%#x\n",cpu.eflags); //debug
//	uint32_t myZF;
//	rtl_get_ZF(&myZF);
//	printf("ZF=%u",myZF);
  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
	rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
	operand_write(id_dest,&id_dest->val);

	uint32_t flag=0;
	rtl_set_CF(&flag);
	rtl_set_OF(&flag);
	rtl_update_ZFSF(&id_dest->val,id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
	rtl_xor(&(id_dest->val),&(id_dest->val),&(id_src->val));
	operand_write(id_dest,&id_dest->val);
	uint32_t flag=0;
	rtl_set_CF(&flag);
	rtl_set_OF(&flag);
	rtl_update_ZFSF(&id_dest->val,id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
	rtl_or(&(id_dest->val),&(id_dest->val),&(id_src->val));
	operand_write(id_dest,&id_dest->val);

	uint32_t flag=0;
	rtl_set_CF(&flag);
	rtl_set_OF(&flag);
	rtl_update_ZFSF(&id_dest->val,id_dest->width);




  print_asm_template2(or);
}

make_EHelper(sar) {
 // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sar(&(id_dest->val),&(id_dest->val),&(id_src->val) );
	operand_write(id_dest,&id_dest->val);
	rtl_update_ZFSF(&(id_dest->val),id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
 // TODO();
  // unnecessary to update CF and OF in NEMU
	rtl_shl(&(id_dest->val),&(id_dest->val),&(id_src->val));
	operand_write(id_dest,&id_dest->val);

	rtl_update_ZFSF(&(id_dest->val),id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
 // TODO();
  // unnecessary to update CF and OF in NEMU
	rtl_shr(&(id_dest->val),&(id_dest->val),&(id_src->val));
	operand_write(id_dest,&id_dest->val);

	rtl_update_ZFSF(&(id_dest->val),id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
 // TODO();
	rtl_not(&(id_dest->val),&(id_dest->val));
	operand_write(id_dest,&id_dest->val);


  print_asm_template1(not);
}

make_EHelper(rol) {
	uint32_t tmp=id_src->val;
	uint32_t OFtmp=0;
	while(tmp!=0) {
		rtl_msb(&t0,&id_dest->val,id_dest->width);
		rtl_shli(&id_dest->val,&id_dest->val,1);
		rtl_add(&id_dest->val,&id_dest->val,&t0);
		tmp--;
	}
	rtl_msb(&t1,&id_dest->val,id_dest->width);
	if(id_src->val==1)
		if(t1!=t0)
						OFtmp=1;
	rtl_set_OF(&OFtmp);
	rtl_set_CF(&t0);
	operand_write(id_dest,&id_dest->val);


}
