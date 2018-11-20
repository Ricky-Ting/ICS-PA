#include "common.h"
#include "syscall.h"
#include "fs.h"

char* NAME_OF_TASK[] = {
  "SYS_exit",
  "SYS_yield",
  "SYS_open",
  "SYS_read",
  "SYS_write",
  "SYS_kill",
  "SYS_getpid",
  "SYS_close",
  "SYS_lseek",
  "SYS_brk",
  "SYS_fstat",
  "SYS_time",
  "SYS_signal",
  "SYS_execve",
  "SYS_fork",
  "SYS_link",
  "SYS_unlink",
  "SYS_wait",
  "SYS_times",
  "SYS_gettimeofday"
};

intptr_t program_break;
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  //printf("do_syscall: I'm here.\n");
  //printf("do_syscall: a:%s %d %d %d\n", NAME_OF_TASK[a[0]], a[1], a[2], a[3]);

  /*
  if(program_break == -1){
    //printf("do_syscall: Enter break.\n");
    extern intptr_t end;
    program_break = end;
  }
  */

  //printf("do_syscall: I'm here.\n");

  switch (a[0]) {
    case SYS_exit: _halt(a[1]); return c;
    case SYS_yield: _yield(); c->GPR1 = 0; return c;
    case SYS_open:
      c->GPR1 = fs_open((char*)a[1], (int)a[2], (int)a[3]);
      return c;
    case SYS_read:
      c->GPR1 = fs_read((int)a[1], (void*)a[2], (size_t)a[3]);
      return c;
    case SYS_write:
      c->GPR1 = fs_write((int)a[1], (void*)a[2], (size_t)a[3]);
      //printf("do_syscall: SYS_write:%#x\n", c->GPR1);
      return c;
      /*
      if(a[1] == 1 || a[1] == 2){
        for(i = 0;i < a[3];i++){
          _putc(((char*)a[2])[i]);       
        }
        c->GPR1 = i;
        //a[0] = a[3];
        //printf("do_syscall: SYS_write:1:%d\n", i);
      }
      else{
        c->GPR1 = fs_write((int)a[1], (void*)a[2], (size_t)a[3]);
      }
      return c;
      */
    case SYS_close:
      c->GPR1 = fs_close((int)a[1]);
      return c;
    case SYS_lseek:
      c->GPR1 = fs_lseek((int)a[1], (size_t)a[2], (int)a[3]);
      //printf("SYS_lseek: %d\n", c->GPR1);
      return c;
    case SYS_brk:
      program_break = a[1];
      c->GPR1 = 0;
      //a[0] = 0;
      /*
      end = a[1];
      a[0] = 0;
      */
      //panic("hhh\n");
      return c;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
