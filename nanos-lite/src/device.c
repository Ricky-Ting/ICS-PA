#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(size_t i=0;i<len;i++) {
		char tmp;
		memcpy(&tmp,(void *)(buf+i),1);
		_putc(tmp);
	}
	return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  //need complete
	char tmp[200];
	printf("In events_read: in\n");
	int key=read_key();
	bool down=false;
	if(key!=_KEY_NONE) {
		if(key & 0x8000) {
			key^=0x8000;
			down=true;
		}			
		if(down) {
			sprintf(tmp,"kd %s\n",keyname[key]);				
		} 
		else
			sprintf(tmp,"ku %s\n",keyname[key]);
	}
	else {
			sprintf(tmp,"t %d\n",uptime());			
	}
	tmp[len]='\0';
	sprintf(buf,tmp);
	return strlen(tmp);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	if(len+offset>128)
					len=128-offset;

	printf("In dispinfo_read: offset=%d, len=%d\n",offset,len);
	memcpy(buf,((void *)dispinfo)+offset,len);		
	return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x,y;
	uint32_t pixel;
	

	for(size_t i=0;i<len;i+=4) {
			memcpy(&pixel,buf+i,4);
			y=offset/screen_width();
			x=offset%screen_width();
			draw_rect(&pixel,x,y,1,1);
			//printf("in fb_write y=%d\n",y);
	}
	return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
	sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
	printf("%s\n",dispinfo);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
