#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i=0;i<len;i++) {
		_putc(((char *)buf)[i]);
		//printf("In serial_write\n");
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
	//printf("In events_read: ");
	int key=read_key();
	bool down=false;
	extern int fg_pcb;
	if(key!=_KEY_NONE) {
		if(key & 0x8000) {
			key^=0x8000;
			down=true;
		}
		if(key==_KEY_F1 ) {
			if(down) {
					fg_pcb=1;
					Log("Change to 1\n");
					_yield();
			}
		}
		else if(key==_KEY_F2) {
						if(down) {
								fg_pcb=2;
								Log("Change to 2\n");
								_yield();
						} 
		} 
		else if(key==_KEY_F3) {
						if(down) {
								fg_pcb=3;
								Log("Change to 3\n");
								_yield();
						} 
		} 
		else {
			if(down) {
				sprintf(buf,"kd %s\n",keyname[key]);				
			} 
			else
				sprintf(buf,"ku %s\n",keyname[key]);
		}
	}
	else {
			sprintf(buf,"t %d\n",uptime());			
	}
	//printf("%s original len=%d",tmp,len);
	
	((char *)buf)[len]='\0';
	//printf("len=%d\n",len);
	
	return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	if(len+offset>128)
					len=128-offset;

	//printf("In dispinfo_read: offset=%d, len=%d\n",offset,len);
	memcpy(buf,((void *)dispinfo)+offset,len);
     ((char*)buf)[len] = '\0';
	return strlen(buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x,y;
	y=(offset/4)/screen_width();
	x=(offset/4)%screen_width();
	draw_rect((uint32_t *)buf,x,y,len/4,1);
			//printf("in fb_write y=%d\n",y);
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
