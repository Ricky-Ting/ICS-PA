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
	if ( ((addr+len-1)&0xfff) < ((addr)&0xfff)  )			
					assert(0);
	else {
					paddr_t paddr = page_translate(addr);
  				return paddr_read(paddr, len);
	}
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
 	if ( ((addr+len-1)&0xfff) < ((addr)&0xfff)  )			
					assert(0);
	else {
	 				paddr_t paddr = page_translate(addr);	
					paddr_write(paddr, data, len);
	}
}

paddr_t page_translate(vaddr_t addr) {
					uint32_t pagedir = addr>>22;
					uint32_t page = (addr>>12)&0x3ff;
					uint32_t offset = (addr)&0xfff;

					uint32_t dir_entry=paddr_read(cpu.CR3+(pagedir)*4, 4 );
					assert(dir_entry&0x1);
					uint32_t table_entry = paddr_read( ((dir_entry)&0xfff) + (page)*4, 4  );
					assert(table_entry&0x1);
					paddr_t ret = (table_entry&0xfff) + offset;
					return ret;

				
}

