#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

#define SCREEN_PORT 0x100

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      uint32_t now_size = inl(SCREEN_PORT);
      info->width = (now_size>>16);
      info->height = now_size & 0xffff;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
      int i;
      int size =  ctl->h*ctl->w;
      for(i = 0;i < size;i++){
        //int x = i*ctl->w/screen_width();
        //int y = i*ctl->h/screen_height()/screen_width(); 
        fb[(ctl->y+i/ctl->w)*screen_width() + ctl->x+i%ctl->w] = ctl->pixels[i];
        //printf("video_write: %#x\t%#x\n", fb[(ctl->y+i/ctl->w)*screen_width() + ctl->x+i%ctl->w], (ctl->y+i/ctl->w)*screen_width() + ctl->x+i%ctl->w);
        /*
         int i;
         int size = screen_width() * screen_height();
         for (i = 0; i < size; i ++) fb[i] = i;
        */
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
