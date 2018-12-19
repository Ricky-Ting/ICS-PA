#include "memory.h"
#include "proc.h"
extern PCB *current;
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
	return 0;
	if(current->max_brk >= new_brk) {
		current->cur_brk=new_brk;
		return 0;
	}
	else {
		int mode =0x1;
		while(current->max_brk < new_brk) {
			void * pageaddr = new_page(1);
			_map(&(current->as), (void *)current->max_brk, pageaddr,mode );
			current->max_brk+=PGSIZE;
		}					
		current->cur_brk=new_brk;
		return 0;
	} 
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
