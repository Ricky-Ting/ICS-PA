#include "proc.h"
#include "fs.h"

#define DEFAULT_ENTRY 0x8048000

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
	int fd=fs_open(filename,0,0);
//	printf("in loader.c fd=%d\n",fd);
	size_t tmp=fs_filesz(fd);
	printf("in loader.c size=%d\n",tmp);
	uint8_t buf;
/*	
	for(size_t  i=0;i<tmp;i++) {
		fs_read(fd,&buf,1);
		memset((void *)(DEFAULT_ENTRY+i),buf,1);
	}

*/	
	
	
	int nrofpage=0;
	int mode=0x1;
	while(tmp>0) {
		void * pageaddr = new_page(1);
		nrofpage++;
		_map(&(pcb->as),( void * )DEFAULT_ENTRY+(nrofpage-1)*PGSIZE,pageaddr,mode);
		for(int i=0;i<4096;i++) {
			fs_read(fd,&buf,1);
			memset(pageaddr+i,buf,1);
			tmp--;
			if(tmp==0)
					break;
		}	
	}
	
	fs_close(fd);
//	printf("in loadder.c out\n");

  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
	//printf("In loader.c %d\n",entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
			
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
	_protect( &(pcb->as));
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
