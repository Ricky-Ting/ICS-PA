#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	printf("In raise_intr: NO=%d\n",NO);
  //TODO();
	//printf("int %d\n",NO);
	rtl_push(&cpu.eflags);
	rtl_push(&cpu.CS);
	rtl_push(&ret_addr);
//	printf("%u\n",cpu.IDTR.high);
	t1=vaddr_read(cpu.IDTR.high+NO*8,4); //low
//	printf("%u\n",t1);
	t2=vaddr_read(cpu.IDTR.high+NO*8+4,4); //high
//	printf("%u\n",t2);
	cpu.CS=t1&0xffff0000;
	t0=(t1&0xffff)+(t2&0xffff0000);
	uint32_t tmp=1<<9;
	cpu.eflags=cpu.eflags & (~tmp);

	rtl_j(t0);
}

void dev_raise_intr() {
	cpu.INTR=true;
}
