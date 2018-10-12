#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
			uint32_t sizetmp=inl(0x100);
      info->width = (sizetmp>>16) & 0xffff;
      info->height = sizetmp & 0xffff;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;

			int i,j,k; 
			k=0;
			for(i=ctl->x;i<ctl->x+(ctl->h);i++)
				for(j=ctl->y;j<ctl->y+(ctl->w);j++) {
					fb[i*screen_width() + j]= (ctl->pixels[k]);				
					k++;			
				}

      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
