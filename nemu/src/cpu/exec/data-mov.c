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
/*
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
*/
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
}/*
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
*/
make_EHelper(leave) {
  //TODO();
	uint32_t Myebp;
	rtl_lr(&Myebp,5,4);
	rtl_sr(4,&Myebp,4);
	rtl_pop(&Myebp);
	rtl_sr(5,&Myebp,4);

  print_asm("leave");
}
/*
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
		uint32_t myAX,myDX;
		rtl_lr(&myAX,0,2);
		if((myAX>>15)&0x1)
				myDX=0xffff;
		else
				myDX=0;
		rtl_sr(2,&myDX,2);

  }
  else {
   // TODO();
		uint32_t myAX,myDX;
		rtl_lr(&myAX,0,1);
		if((myAX>>7)&0x1)
				myDX=0xff;
		else
				myDX=0;
		rtl_sr(2,&myDX,1);

  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}
*/

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
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
