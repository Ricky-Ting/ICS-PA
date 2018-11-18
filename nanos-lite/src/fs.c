#include "fs.h"
#include "proc.h"


typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
	size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
	{"/dev/fb",0,0,invalid_read,fb_write},
	{"/proc/dispinfo",0,0,dispinfo_read,invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode) {
	printf("fs_open: %s\n",pathname);
	for(int i=3;i<NR_FILES;i++)
				 	if(strcmp(pathname,file_table[i].name)==0) {
						file_table[i].open_offset=0;
						return i;
					}
	panic("fs_open: should not reach here!");
}

size_t fs_filesz(int fd) {
	return file_table[fd].size;				
}

ssize_t fs_read(int fd,void *buf, size_t len) {
	if(file_table[fd].read!=NULL) {
					size_t tmp=file_table[fd].read(buf,file_table[fd].open_offset,len);
					file_table[fd].open_offset+=tmp;
					return tmp;
	}
	if(fd>=NR_FILES)
					panic("In fs_read: Wrong fd");
	if(file_table[fd].open_offset+len>file_table[fd].size) {
					len=file_table[fd].size-file_table[fd].open_offset;
					ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
					file_table[fd].open_offset=file_table[fd].size;
					return len;
	} 
	else { 
					//printf("In fs_read, off:%d len:%d\n",file_table[fd].open_offset,len);
					
					ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
//			printf("in fs_read, set=%d\n",file_table[fd].open_offset+file_table[fd].disk_offset);
					file_table[fd].open_offset+=len;
					return len;
	}
}

ssize_t fs_write(int fd, const void * buf, size_t len) {
	if(file_table[fd].write!=NULL) {
					size_t tmp=file_table[fd].write(buf,file_table[fd].open_offset,len);
					file_table[fd].open_offset+=tmp;
					return tmp;
					
	}
	if(fd>=NR_FILES)
					panic("In fs_write: Wrong fd");
	if(file_table[fd].open_offset+len>file_table[fd].size)
					panic("In fs_write: Too much request");
	else {
					ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
					file_table[fd].open_offset+=len;
					return len;
	} 
}

off_t fs_lseek(int fd, off_t offset, int whence) {
	if(fd>=NR_FILES)
			panic("fs_lseek: Wrong fd!");
	switch(whence) {
		case SEEK_SET: 
							if(offset>file_table[fd].size) {
										//	printf("In fs_lseek: offset=%d, size=%d\n",offset, file_table[fd].size);
											panic("fs_lseek: out of the file");
							}
							else {
										//	printf("In seekset, offset=%d",offset);
											file_table[fd].open_offset=offset;
											return file_table[fd].open_offset;
							}
							break;
		case SEEK_CUR:
							if(offset+file_table[fd].open_offset>file_table[fd].size)
											panic("fs_lseek: out of the file");
							else{
											//printf("In seekcur, offset=%d",offset);
	
											return file_table[fd].open_offset=file_table[fd].open_offset+offset;
											
							}
							break;
		case SEEK_END:
							if(offset>0)
											panic("fs_lseek: Should not reach here(SEEK_END)");
							else{
										//printf("In seekend, offset=%d, return %d",offset,file_table[fd].size+offset);
	

											return file_table[fd].open_offset=file_table[fd].size+offset; 
							}
							break;
		default: panic("fs_lseek: Should not reach here"); break;
	}				
}

int fs_close(int fd) {
		if(fd>=NR_FILES)
				panic("fs_close: Wrong fd");
		else {
				file_table[fd].open_offset=0;
				return 0;
		}

}


void init_fs() {
  // TODO: initialize the size of /dev/fb
	int i;
	for(i=0;i<NR_FILES;i++) {
		if(strcmp(file_table[i].name,"/dev/fb")==0)
						break;
	}
	file_table[i].size=screen_width()*screen_height()*4;
}
