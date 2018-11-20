#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i = 0;i < len;i++){
    _putc(((char*)buf)[i]);
  } 
  return len;
  //strncpy((char*)0x3f8, buf, len);
  //return strlen((char*)0x38);
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

//size_t input_read(uintptr_t reg, void *buf, size_t size);
size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  bool down = false;
  if(key & 0x8000){
    key ^= 0x8000;
    down = true;
  }
  if(key != _KEY_NONE){
    sprintf(buf, "%s %s\n", down?"kd":"ku", keyname[key]);
  }
  else{
    sprintf(buf, "t %d\n", uptime());
  }
  if(strlen(buf) > len){
    printf("\33[1;31mERROR: In events_read: buf is too long.\33[0m\n");
    assert(0);
  }
  ((char*)buf)[len] = '\0';
  return strlen(buf);
  //return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  /*
  if(len >= 128)
    len = 127;
  */
  for(int i = 0;i < len;i++){
    ((char*)buf)[i] = dispinfo[offset+i];
  }
  ((char*)buf)[len] = '\0';
  //return len;
  //printf("dispinfo_read: len:%d\tstrlen:%d\n", len, strlen(buf));
  return strlen(buf);
}

//size_t video_read(uintptr_t reg, void *buf, size_t size);
//void draw_rect(uint32_t *pixels, int x, int y, int w, int h);
uint32_t buff[1<<16];
size_t fb_write(const void *buf, size_t offset, size_t len) {
  /*
  uint32_t size[2];
  //printf("fb_write: I'm here.\n");
  video_read(1, size, 8);
  */
  //printf("fb_write: %p\n", buf);
  //draw_rect((uint32_t*)buf, offset%(screen_width()), offset/(screen_width()), len, 1);
  //draw_rect((uint32_t*)buf, offset%screen_width(), offset/(screen_width()), len, 1);
  //draw_rect((uint32_t*)buf, 0, offset/(screen_width()), len, 1);
  /*
  for(int i = 0;i < len;i++){
    printf("%d ", ((uint32_t*)buf)[i]);
  }
  printf("\n");
  */
  offset /= 4;
  //char term = ((char*)buf)[len];
  //((char*)buf)[len] = '\0';
  /*
  for(int i = 0;i < len;i++){
    buff[i] = ((uint32_t*)buf)[i];
  }
  buff[len] = '\0';
  */
  //draw_rect((uint32_t*)buf, offset%screen_width(), offset/screen_width(), screen_width(), screen_height());
  draw_rect((uint32_t*)buf, offset%screen_width(), offset/screen_width(), len/4, 1);
  //((char*)buf)[len] = term;
  //printf("fb_write: offset:%#x\n", offset);
  //draw_sync();
  //printf("fb_write: len:%d\n", len);
  //printf("fb_write: size0f(buf):%d\n", sizeof(buf));
  return len;
  //return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  /*
  uint32_t size[2];
  video_read(1, size, 8);
  memset(dispinfo, 0, sizeof(dispinfo));
  */
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
}
