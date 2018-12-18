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
  //TODO();
	if(id_dest->width==2) {
		rtl_lr(&t1,4,2); //sp
		rtl_lr(&t0,0,2); //ax
		rtl_push(&t0); //push ax
		rtl_lr(&t0,1,2); //cx
		rtl_push(&t0); //push cx
		rtl_lr(&t0,2,2); //dx
		rtl_push(&t0); //push dx
		rtl_lr(&t0,3,2); //bx
		rtl_push(&t0); //push bx
		rtl_lr(&t0,4,2); //sp
		rtl_push(&t0); //push sp
		rtl_lr(&t0,5,2); //bp
		rtl_push(&t0); //push bp
		rtl_lr(&t0,6,2); //si
		rtl_push(&t0); //push si
		rtl_lr(&t0,7,2); //di
		rtl_push(&t0); //push di



	} else {
		rtl_lr(&t1,4,4); //esp
		rtl_lr(&t0,0,4); //eax
		rtl_push(&t0); //push eax
		rtl_lr(&t0,1,4); //ecx
		rtl_push(&t0); //push ecx
		rtl_lr(&t0,2,4); //edx
		rtl_push(&t0); //push edx
		rtl_lr(&t0,3,4); //ebx
		rtl_push(&t0); //push ebx
		rtl_lr(&t0,4,4); //esp
		rtl_push(&t0); //push esp
		rtl_lr(&t0,5,4); //ebp
		rtl_push(&t0); //push ebp
		rtl_lr(&t0,6,4); //esi
		rtl_push(&t0); //push esi
		rtl_lr(&t0,7,4); //edi
		rtl_push(&t0); //push edi
				
	}

  print_asm("pusha");
}



make_EHelper(popa) {
  //TODO();
		if(id_dest->width==2) {
		rtl_pop(&t0); //pop di 
		rtl_sr(7,&t0,2); //di
		rtl_pop(&t0); //pop si
		rtl_sr(6,&t0,2); //si
		rtl_pop(&t0); //pop bp
		rtl_sr(5,&t0,2); //bp
		rtl_pop(&t0); //pop sp
		//rtl_sr(4,&t0,4,2); //sp
		rtl_pop(&t0); //pop bx
		rtl_sr(3,&t0,2); //bx
		rtl_pop(&t0); //pop dx
		rtl_sr(2,&t0,2); //dx
		rtl_pop(&t0); //pop cx
		rtl_sr(1,&t0,2); //cx
		rtl_pop(&t0); //pop ax
		rtl_sr(0,&t0,2); //ax



	} else {
		rtl_pop(&t0); //pop edi 
		rtl_sr(7,&t0,4); //edi
		rtl_pop(&t0); //pop esi
		rtl_sr(6,&t0,4); //esi
		rtl_pop(&t0); //pop ebp
		rtl_sr(5,&t0,4); //ebp
		rtl_pop(&t0); //pop esp
		//rtl_sr(4,&t0,4,4); //esp
		rtl_pop(&t0); //pop ebx
		rtl_sr(3,&t0,4); //ebx
		rtl_pop(&t0); //pop edx
		rtl_sr(2,&t0,4); //edx
		rtl_pop(&t0); //pop ecx
		rtl_sr(1,&t0,4); //ecx
		rtl_pop(&t0); //pop eax
		rtl_sr(0,&t0,4); //eax


			
	}


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
   // TODO();
		uint32_t myAX;
        rtl_lr(&myAX,0,2);
		rtl_sext(&t1,&myAX,2);
        myAX=(t1&0xffff) | myAX;
		rtl_sr(0,&myAX,4);

  }
  else {
   // TODO();
      uint32_t myAX;
      rtl_lr(&myAX,0,2);
      rtl_sext(&t1,&myAX,4);
      myAX=t1;
      rtl_sr(0,&myAX,4);

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

make_EHelper(mov_G2C) {
	if(id_dest->reg==0) 
					cpu.CR0=id_src->val;
	else if(id_dest->reg==3) 
					cpu.CR3=id_src->val;
	else
					assert(0);
	print_asm_template2(movC);
}
