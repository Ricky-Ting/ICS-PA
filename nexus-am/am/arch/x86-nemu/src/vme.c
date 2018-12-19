#include <x86.h>
#include <klib.h>
#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

	printf("In vme_init\n");
  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }
	//printf("In vme_init here\n");
  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }
	//printf("In vme_init here here\n");
  set_cr3(kpdirs);
	//printf("In vme_init set cr3 to %d\n",&kpdirs);
  set_cr0(get_cr0() | CR0_PG);
	//printf("sre cr0\n");
	printf("outof init\n");
  return 0;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect * p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c ) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
} 

int _map(_Protect *p, void *va, void *pa, int mode) {
	PDE *updir = p->ptr;
	if( !( updir[PDX(va)] & PTE_P)  ) {
		uint32_t newpageaddr = (uint32_t)pgalloc_usr(1);
		updir[PDX(va)] = (newpageaddr & ~0xfff) | PTE_P;
		printf("In map : %x\n",updir[PDX(va)]);
	}

	PTE *this_page_table =  (PTE *)((uint32_t)( updir[PDX(va)] ) & ~0xfff);
	this_page_table[PTX(va)]=    ((uint32_t)pa & ~0xfff)        |( mode & 0xfff);
	
		
  return 0;
}

_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
				ustack.end -= 1 * sizeof(uintptr_t);
				uintptr_t ret = (uintptr_t) ustack.end;
				*(uintptr_t *) ret =0;

				_Context tmp;
				tmp.cs=8;
				tmp.eip=(uintptr_t)entry;
				tmp.prot=p;
			
				memcpy( (void *)(ustack.end-sizeof(tmp)), (void *)(&tmp), sizeof(tmp)  );
				return (ustack.end-sizeof(tmp));


}
