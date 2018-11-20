#include <dlfcn.h>

#include "nemu.h"
#include "monitor/monitor.h"
#include "diff-test.h"

static void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n);
static void (*ref_difftest_getregs)(void *c);
static void (*ref_difftest_setregs)(const void *c);
static void (*ref_difftest_exec)(uint64_t n);

static bool is_skip_ref;
static bool is_skip_dut;

void difftest_skip_ref() { is_skip_ref = true; }
void difftest_skip_dut() { is_skip_dut = true; }

void init_difftest(char *ref_so_file, long img_size) {
#ifndef DIFF_TEST
  return;
#endif

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_DEEPBIND);
  assert(handle);

  ref_difftest_memcpy_from_dut = dlsym(handle, "difftest_memcpy_from_dut");
  assert(ref_difftest_memcpy_from_dut);

  ref_difftest_getregs = dlsym(handle, "difftest_getregs");
  assert(ref_difftest_getregs);

  ref_difftest_setregs = dlsym(handle, "difftest_setregs");
  assert(ref_difftest_setregs);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  void (*ref_difftest_init)(void) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: \33[1;32m%s\33[0m", "ON");
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in include/common.h.", ref_so_file);

  ref_difftest_init();
  ref_difftest_memcpy_from_dut(ENTRY_START, guest_to_host(ENTRY_START), img_size);
  ref_difftest_setregs(&cpu);
}

char* g_r_s(int index){
  static char* cpu_string[14] = {"$eax", "$ecx", "$edx", "$ebx", "$esp", "$ebp", "$esi", "$edi", "$eip", "CF", "ZF", "SF", "IF", "OF"};
  return cpu_string[index];
} 

void difftest_step(uint32_t eip) {
  CPU_state ref_r;

  if (is_skip_dut) {
    is_skip_dut = false;
    return;
  }

  if (is_skip_ref) {
    // to skip the checking of an instruction, just copy the reg state to reference design
    ref_difftest_setregs(&cpu);
    is_skip_ref = false;
    return;
  }

  ref_difftest_exec(1);
  ref_difftest_getregs(&ref_r);

  // TODO: Check the registers state with the reference design.
  // Set `nemu_state` to `NEMU_ABORT` if they are not the same.
  //TODO();
  extern CPU_state cpu;
  for(int i = 0;i < 8;i++){
    if(cpu.gpr[i]._32 != ref_r.gpr[i]._32){
      printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(i), ref_r.gpr[i]._32, g_r_s(i), cpu.gpr[i]._32);
      nemu_state = NEMU_ABORT;
    }
  }

  if(cpu.eip != ref_r.eip){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(8), ref_r.eip, g_r_s(8), cpu.eip);
    nemu_state = NEMU_ABORT;
  }
  /*
  if(cpu.IDTR.B != ref_r.IDTR.B || cpu.IDTR.L != ref_r.IDTR.L){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:IDTR.B:%#x\tcpu:IDTR.B:%#x\tref_r:IDTR.L:%#x\tcpu:IDTR.L:%#x\n", eip, ref_r.IDTR.B, cpu.IDTR.B, ref_r.IDTR.L, cpu.IDTR.L);
    nemu_state = NEMU_ABORT;
  }
  */
  /*
  if(cpu_CF() != (ref_r.EFLAGS & 1)){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(9), cpu_CF(), g_r_s(9), (ref_r.EFLAGS & 1));
    nemu_state = NEMU_ABORT;
  }

  if(cpu_ZF() != (ref_r.EFLAGS & 6)){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(10), cpu_ZF(), g_r_s(10), (ref_r.EFLAGS & 6));
    nemu_state = NEMU_ABORT;
  }

  if(cpu_SF() != (ref_r.EFLAGS & 7)){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(11), cpu_ZF(), g_r_s(11), (ref_r.EFLAGS & 7));
    nemu_state = NEMU_ABORT;
  }

  if(cpu_IF() != (ref_r.EFLAGS & 9)){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(12), cpu_IF(), g_r_s(12), (ref_r.EFLAGS & 9));
    nemu_state = NEMU_ABORT;
  }

  if(cpu_OF() != (ref_r.EFLAGS & 11)){
    printf("\33[1;31mdiff_test:different happen at eip:%#x, ref_r:%s:%#x\tcpu:%s:%#x\n", eip, g_r_s(13), cpu_OF(), g_r_s(13), (ref_r.EFLAGS & 11));
    nemu_state = NEMU_ABORT;
  }
  */
}
