#include "common.h"
#include "syscall.h"

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
													//_putc(tmp);
													printf("%d %d",(tmp),a[3]);
												}
												c->GPRx=a[3];
								}
								else 
											panic("SYS_write need to be implemented\n");
								break;
		case SYS_exit: /*printf("in\n");*/ _halt(c->GPR2); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
