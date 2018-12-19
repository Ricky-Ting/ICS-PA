#include "common.h"
#include "syscall.h"
#include "proc.h"
_Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
		case _EVENT_SYSCALL:  do_syscall(c); break;
		case _EVENT_YIELD :  return schedule(c);    break;
		case _EVENT_IRQ_TIMER:/* Log("This is _EVENT_IRQ_TIMER\n");*/ _yield(); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
