#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(void) {
	if(free_==NULL)
		assert(0);
	WP *tmp = free_;
	free_=free_->next;
	
	tmp->next=head;
	head=tmp;
	return tmp;
}

void free_wp(WP *wp) {
	WP* tmp=head;
	while(tmp->next!=wp)
		tmp=tmp->next;
	tmp->next=wp->next;
	wp->next=free_;
	free_=wp;
}

void delete_wp(int num) {
	WP *tmp=head;
	while(tmp!=NULL && tmp->NO!=num)
		tmp=tmp->next;
	if(tmp==NULL) {
		printf("Watchpoint %d doesn't exisit\n",num);
		assert(0);
	}
	free_wp(tmp);
	printf("Watchpoint %d has been deleted\n",num);
	return;
}
