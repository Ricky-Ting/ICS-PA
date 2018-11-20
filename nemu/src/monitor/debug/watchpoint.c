#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <string.h>

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

char watchpoint_list[1<<16];

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

WP* new_wp(char *expression){
	if(free_ == NULL){
		Log("Error: The number of watchpoint is out of the upper limit.");
		assert(0);
	}

	int Value = 0;
	bool success = true;
	Value = expr(expression, &success);
	if(!success){
		printf("表达式非法，创建监视点失败\n");
		return NULL;
	}
	
	if(head == NULL){
		head = free_;
		free_ = free_->next;
		head->next = NULL;
		head->NO = 1;
		head->expression[0] = '\0';
		strcpy(head->expression, expression);
		head->value = Value;
		return head;
	}

	WP* Pointer = head;
	while(Pointer->next != NULL)
		Pointer = Pointer->next;

	Pointer->next = free_;
	free_ = free_->next;
	Pointer->next->next = NULL;
	Pointer->next->NO = Pointer->NO + 1;
	Pointer->next->expression[0] = '\0';
	strcpy(Pointer->next->expression, expression);
	Pointer->value = Value;
	return Pointer->next;
}

void free_wp(WP *pre_wp, WP *wp){
	if(pre_wp == NULL){
		WP *term;
		term = head;
		head = head->next;
		term->next = free_;
		free_ = term; 
		return;
	}
	else if(wp->next == NULL){
		pre_wp->next = NULL;
		wp->next = free_;
		free_ = wp;
		return;
	}
	else{
		pre_wp->next = wp->next;
		wp->next = free_;
		free_ = wp;
		return;
	}
	/*
	WP* Pointer = wp->next;
	while(Pointer != NULL){
		Pointer->No--;
		Pointer = Pointer->next;
	}
	if(wp->next == NULL){
		
	}
	WP term1 = *wp;
	WP* term2 = wp->next;
	*wp = *(wp->next);
	*term2 = term1;
	term2->next = free_;
	free_ = term2;
	*/
}

char* show_watchpoint(int order){
	watchpoint_list[0] = '\0';
	if(order == 0){
		WP* Pointer = head;
		char term[1024];
		term[0] = '\0';
		while(Pointer != NULL){
			sprintf(term, "序号:%d\t表达式:%s\t值:%d\n", Pointer->NO, Pointer->expression, Pointer->value);
			strcat(watchpoint_list, term);
			Pointer = Pointer->next;
		}
	}
	else{
		WP* Pointer = head;
		char term[1024];
		term[0] = '\0';
		while(Pointer != NULL){
			if(Pointer->NO == order)
				continue;
			sprintf(term, "序号:%d\t表达式:%s\n", Pointer->NO, Pointer->expression);
			strcat(watchpoint_list, term);
			break;
		}
	}
	return watchpoint_list;
}

bool delete_watchpoint(int order){
	WP* Pointer = head;
	WP* pre_P = NULL;
	while(Pointer != NULL){
		if(Pointer->NO != order){
			pre_P = Pointer;
			Pointer = Pointer->next;
			continue;
		}
		free_wp(pre_P, Pointer);
		return true;
	}
	return false;
}

bool is_change(){
	WP *Pointer = head;
	bool success = true;
	int Value = 0;
	bool no_change = true;
	while(Pointer != NULL){
		Value = expr(Pointer->expression, &success);
		if(Value == Pointer->value && success){
		}
		else{
			printf("检查点%d改变，原值：%d\t现值：%d\n",Pointer->NO, Pointer->value, Value);
			Pointer->value = Value;
			no_change = false;
		}
		Pointer = Pointer->next;
		continue;
	}
	return no_change;
}
