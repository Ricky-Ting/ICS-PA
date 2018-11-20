#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  //printf("raise_intr: I start the work now\n");
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  /*
  if(id_dest->val > cpu.IDTR.L){
    panic("Mistake happens in \"int\": NO is larger than then length of table.\tid_dest->val:%d\tcpu.IDTR.L:%d\n", id_dest->val, cpu.IDTR.L);
  }
  */
  rtl_push(&cpu.eflags);
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  
  //printf("raise_intr: ret_addr:%#x\n", ret_addr);
  //printf("raise_intr: NO:%#x\n", NO);
  /*
  for(int i = 0;i <= NO+2;i++){
  printf("raise_intr: NO:%#x\thigh:%#x\tlow:%#x\n", i, vaddr_read(cpu.IDTR.B+8*i+6, 2), vaddr_read(cpu.IDTR.B+8*i, 2));
  }
  */

  //printf("raise_intr: I am going to get t0.\n");
  //printf("raise_intr: high:%#x\tlow:%#x\n", vaddr_read(cpu.IDTR.B+8*NO+6, 2), vaddr_read(cpu.IDTR.B+8*NO, 2));
  t0 = (vaddr_read(cpu.IDTR.high+8*NO+6, 2) << 16) + vaddr_read(cpu.IDTR.high+8*NO, 2);
  //printf("raise_intr: t0:%#x\n", t0);
  rtl_j(t0);
  //TODO();
}

void dev_raise_intr() {
}
