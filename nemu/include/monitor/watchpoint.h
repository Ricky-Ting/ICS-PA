#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
	char expression[1024];
	int value;
} WP;

WP* new_wp(char *expression);

void free_wp(WP *pre_wp, WP *wp);

char* show_watchpoint(int order);

bool delete_watchpoint(int order);

bool is_change();
#endif
