#include "common.h"
#include "syscall.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  switch (a[0]) {
		case SYS_yield: _yield(); c->GPRx=0; break;
		case SYS_write: 
								if(a[1]==1 || a[1]==2) {
												for(size_t i=0;i<a[3];i++) {
													char tmp;
													memcpy(&tmp,(void *)(a[2]+i),1);
													_putc(tmp);
												}
												c->GPRx=a[3];
								}
								else 
												c->GPRx=fs_write(a[1],(void *)a[2],a[3]);
								break;
		case SYS_read:
								if(a[1]==0)
												panic("syscall: stdin has not been implemented");
								else
												c->GPRx=fs_read(a[1],(void *)a[2],a[3]);
								break;
		case SYS_close: c->GPRx=fs_close(a[1]); break;
		case SYS_lseek: c->GPRx=fs_lseek(a[1],a[2],a[3]); break;
		case SYS_open: c->GPRx=fs_open((const char *)a[1],a[2],a[3]); break;

		case SYS_brk: c->GPRx=0;  break;

		
		case SYS_exit: /*printf("in\n");*/ _halt(c->GPR2); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
