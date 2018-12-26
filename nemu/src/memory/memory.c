#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];
paddr_t page_translate(vaddr_t addr);
/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	//printf("In paddr_read: addr=%x\n",addr);
	if(is_mmio(addr)==-1) 
  	return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
	else
		return mmio_read(addr,len,is_mmio(addr));
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
	if(is_mmio(addr)==-1)
  	memcpy(guest_to_host(addr), &data, len);
	else
		mmio_write(addr,len,data,is_mmio(addr));
}

uint32_t vaddr_read(vaddr_t addr, int len) {
	if(!(  (cpu.CR0>>31)   ) )
					return paddr_read(addr,len);

	if ( ((addr+len-1)&0xfff) < ((addr)&0xfff)  )	 {	
					//assert(0);
					uint32_t len2= (addr+len)&0xfff;
					uint32_t len1= len - len2;
					uint32_t data1=vaddr_read(addr,len1);
					uint32_t data2=vaddr_read(addr+len1,len2);
					uint32_t ret = (data2<<(len1*8)) + data1;
					return ret;

	}
	else {
					paddr_t paddr = page_translate(addr);
  				return paddr_read(paddr, len);
	}
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {

	if(!(   (cpu.CR0>>31)   ) )
					return paddr_write(addr,data,len);
 	if ( ((addr+len-1)&0xfff) < ((addr)&0xfff)  ) {			
					//assert(0);
					uint32_t len2= (addr+len)&0xfff;
					uint32_t len1= len - len2;
					printf("In\n");	
					uint32_t data1,data2;
					if(len1==1) 
									data1 = data&0xff;
					else if(len1==2) 
									data1 = data&0xffff;
					else if(len1==3)
									data1 = data&0xffffff;
					else
									assert(0);
					
					data2=data>>(len1*8);
	
					vaddr_write(addr,data1,len1);
					vaddr_write(addr+len1,data2,len2);
					return ;

	}
	else {
	 				paddr_t paddr = page_translate(addr);	
					paddr_write(paddr, data, len);
	}
}

paddr_t page_translate(vaddr_t addr) {
					uint32_t pagedir = addr>>22;
					uint32_t page = (addr>>12)&0x3ff;
					uint32_t offset = (addr)&0xfff;
//			if(addr==0x804a000)	{	printf("In page_translate: aadr=%x\n",addr);
//					printf("In page translate: pagedir=%x\n",pagedir);
//					printf("In page translate: page=%x\n",page);
//					printf("In page translate: offset=%x\n",offset); }
					uint32_t dir_entry=paddr_read(cpu.CR3+(pagedir)*4, 4 );
//			if(addr==0x804a000)		{printf("In page_translate : cr3=%x\n",cpu.CR3);
//					printf("In page_translate: dir_entry=%x\n",dir_entry);}
					assert(dir_entry&0x1);
					uint32_t table_entry = paddr_read( ((dir_entry)& ~0xfff) + (page)*4, 4  );
				//	printf("In page_translate: table_entry=%x\n",table_entry);
				//	assert(table_entry&0x1);
				if(!(table_entry & 0x1)) {
						printf("In translate addr=%x\n",addr);
						assert(0);
								
					}
					paddr_t ret = (table_entry&~0xfff) + offset;
			//		assert(ret==addr);
//			if(addr==0x804a000)  {printf("In page translate, ret=%x\n",ret);assert(0); }
				//	printf("\n");
					return ret;

				
}

