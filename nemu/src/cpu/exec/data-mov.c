#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
	rtl_push(&id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
 // TODO();
	rtl_pop(&id_dest->val);	
	operand_write(id_dest,&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
	uint32_t Myebp;
	rtl_lr(&Myebp,5,4);
	rtl_sr(4,&Myebp,4);
	rtl_pop(&Myebp);
	rtl_sr(5,&Myebp,4);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    //TODO();
		uint32_t myAX,myDX;
		rtl_lr(&myAX,0,2);
		if((myAX>>15)&0x1)
				myDX=0xffff;
		else
				myDX=0;
		rtl_sr(2,&myDX,2);
  }
  else {
    //TODO();
		uint32_t myEAX,myEDX;
		rtl_lr(&myEAX,0,4);
		if((myEAX>>31)&0x1)
				myEDX=0xffffffff;
		else
				myEDX=0;
		rtl_sr(2,&myEDX,4);

  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
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
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
