#include "cpu/exec.h"
#include "device/port-io.h" //myadd

extern void raise_intr(uint8_t NO, vaddr_t ret_addr);

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  //TODO();
//	printf("%u\n",id_dest->addr);
//	printf("%u\n",vaddr_read(id_dest->addr+2,4));
//	printf("%u\n",vaddr_read(0x0012C020,4));
	rtl_lm(&t1,&id_dest->addr,2);
	t0=id_dest->addr+2;
	rtl_lm(&t2,&t0,4);
	if(id_dest->width==2) {
		cpu.IDTR.low=t1&0xffff;
		cpu.IDTR.high=t2&0xffffff;
	} else {
		cpu.IDTR.low=t1&0xffff;
		cpu.IDTR.high=t2;
	}
	//printf("h%u\n",cpu.IDTR.high);
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //TODO();
	if(id_dest->reg==0) {
					cpu.CR0 = id_src->val;
					printf("cr0:%x\n",id_src->val);
	}
	else if(id_dest->reg==3) {
					cpu.CR3 = id_src->val;
					printf("cr3:%x\n",id_src->val);
	}
	else
					assert(0);
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //TODO();
	if(id_src->reg==0) 
					id_src->val=cpu.CR0;
	else if(id_src->reg==3)
					id_src->val=cpu.CR3;
	else
					assert(0);
	operand_write(id_dest,&id_src->val);
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  //TODO();
	//printf("%u\n",id_dest->val);
	raise_intr(id_dest->val&0xff,*eip);
	//printf("eip=%#x\n",cpu.eip);
  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  //TODO();
	rtl_pop(&t0);
	

	rtl_pop(&cpu.CS);
	rtl_pop(&cpu.eflags);
    rtl_j(t0);
	
  print_asm("iret");
}

make_EHelper(in) {
  //TODO();
	switch(id_dest->width) {
		case 1: t0=pio_read_b(id_src->val); break;
		case 2: t0=pio_read_w(id_src->val); break;
		case 4: t0=pio_read_l(id_src->val); break;
		default: assert(0);
	}
	rtl_sr(id_dest->reg,&t0,id_dest->width);
  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  //TODO();
		rtl_lr(&t0,id_src->reg,id_src->width);
		switch(id_src->width) {
			case 1: pio_write_b(id_dest->val,t0); break;
			case 2: pio_write_w(id_dest->val,t0); break;
			case 4: pio_write_l(id_dest->val,t0); break;
			default: assert(0);
		}
  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
