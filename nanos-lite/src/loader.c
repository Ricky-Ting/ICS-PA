#include "proc.h"
#include "common.h"

#define DEFAULT_ENTRY 0x4000000

//#define BUFF_SIZE 1024
//char buff[BUFF_SIZE];

size_t get_ramdisk_size();
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t fs_filesz(int fd);
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  int NO = fs_open(filename, 0, 0);
  fs_read(NO, (void*)DEFAULT_ENTRY, fs_filesz(NO));
  //printf("loader: I'm here.\n");
  //ramdisk_read((void*)DEFAULT_ENTRY, 0, get_ramdisk_size());
  //printf("lodder: get_ramdisk_size():%#x\n", get_ramdisk_size());
  /*
  size_t s = get_ramdisk_size();  
  int i;
  for(i = 0;(i+1)*BUFF_SIZE <= s;i++){
    ramdisk_read(buff, i*BUFF_SIZE, BUFF_SIZE);
    memcpy((void*)(DEFAULT_ENTRY+i*BUFF_SIZE), buff, BUFF_SIZE);
  }
  ramdisk_read(buff, i*BUFF_SIZE, s-BUFF_SIZE*i);
  memcpy((void*)(DEFAULT_ENTRY+i*BUFF_SIZE), buff, s-BUFF_SIZE*i);
  */
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
  //printf("naive_uload: I'm here\n");
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
