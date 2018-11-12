#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
	rtl_push(&cpu.eflags);
	rtl_push(&cpu.CS);
	rtl_push(&ret_addr);
	printf("%u",cpu.IDTR.low);
	t1=vaddr_read(cpu.IDTR.low+NO*8,4); //low
	t2=vaddr_read(cpu.IDTR.low+NO*8+4,4); //high
	cpu.CS=t1&0xffff0000;
	t0=(t1&0xffff)+(t2&0xffff0000);
	rtl_j(t0);
}

void dev_raise_intr() {
}
