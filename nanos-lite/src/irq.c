#include "common.h"

_Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
  //printf("do_event: I'm here.\n");
  //printf("do_event: e.event:%d\n", e.event);
  switch (e.event) {
    //case _EVENT_SYSCALL: printf("\033[36mEnter \"_EVENT_SYSCALL\"\033[0m\n"); break;
    case _EVENT_SYSCALL: do_syscall(c); break;
    case _EVENT_YIELD: printf("\033[36mDo you like what you see?\033[0m\n"); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
