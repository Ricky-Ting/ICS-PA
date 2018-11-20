#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  //TODO();
  cpu.IDTR.L = vaddr_read(id_dest->addr, 2);
  cpu.IDTR.B = vaddr_read(id_dest->addr+2, 4);
  //printf("lidt:\tB:%#x\tL:%#x\n", cpu.IDTR.B, cpu.IDTR.L);
  if(id_dest->width == 2)
    cpu.IDTR.B &= 0xffffff;

  //printf("lidt: I arrive the final.\n");
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

void raise_intr(uint8_t NO, vaddr_t ret_addr);
make_EHelper(int) {
  //TODO();
  /*
  if(id_dest->val > cpu.IDTR.L){
    panic("Mistake happens in \"int\": NO is larger than then length of table.");
  }
  rtl_push(&cpu.EFLAGS);
  rtl_push(&cpu.CS);
  rtl_push(&cpu.eip);
  */
  //printf("int: *eip:%#x\tcpu.eip:%#x\n", *eip, cpu.eip);
  raise_intr(id_dest->val, *eip);

  //assert(0);

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  //TODO();
  rtl_pop(&t0);
  //*eip = t0;
  rtl_pop(&cpu.CS);
  rtl_pop(&cpu.EFLAGS);
  //printf("iret: t0:%#x\tCS:%#x\tEFLAGS:%#x\n", t0, cpu.CS, cpu.EFLAGS);
  rtl_j(t0);

  //printf("iret: t0:%#x\n", t0);
  //assert(0);
  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t addr);
uint32_t pio_read_w(ioaddr_t addr);
uint32_t pio_read_b(ioaddr_t addr);
make_EHelper(in) {
  //TODO();
  if(id_src->width == 4) 
    id_dest->val = pio_read_l(id_src->val);
  else if(id_src->width == 2)
    id_dest->val = pio_read_w(id_src->val);
  else
    id_dest->val = pio_read_b(id_src->val);
  //printf("in:\t%#x\t%#x\t%#x\n", id_src->val, pio_read_l(id_src->val), id_dest->val);
  operand_write(id_dest, &id_dest->val); 

  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

void pio_write_l(ioaddr_t addr, uint32_t data); 
void pio_write_w(ioaddr_t addr, uint32_t data); 
void pio_write_b(ioaddr_t addr, uint32_t data); 
make_EHelper(out) {
  //TODO();
  if(id_src->width == 4)
    pio_write_l(id_dest->val, id_src->val);
  else if(id_src->width == 2)
    pio_write_w(id_dest->val, id_src->val);
  else
    pio_write_b(id_dest->val, id_src->val);

  //printf("out:\t%#x\t%#x\n", id_src->val, id_dest->val);

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
