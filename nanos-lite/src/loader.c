#include "proc.h"

#define DEFAULT_ENTRY 0x4000000

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
	size_t tmp=get_ramdisk_size();
	uint8_t buf;
	for(size_t  i=0;i<tmp;i++) {
		ramdisk_read(&buf,i,1);
		ramdisk_write(&buf,DEFAULT_ENTRY+i,1);
	}

  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
