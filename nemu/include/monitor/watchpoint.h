#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
	char  e[65536];
	uint32_t value;

} WP;

WP* new_wp(void);
void free_wp(WP *wp);
void delete_wp(int num);
void walk(void);
bool testify(void);

#endif
