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
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
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
	if(fd>=NR_FILES)
					panic("In fs_read: Wrong fd");
	if(file_table[fd].open_offset+len>file_table[fd].size)
					panic("In fs_read: Too much request");
	else { 
					
					ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
				/*	printf("in fs_read, set=%d\n",file_table[fd].open_offset+file_table[fd].disk_offset); */
					file_table[fd].open_offset+=len;
					return len;
	}
}

ssize_t fs_write(int fd, const void * buf, size_t len) {
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
							if(offset>=file_table[fd].size)
											panic("fs_lseek: out of the file");
							else
											return file_table[fd].open_offset=offset;
							break;
		case SEEK_CUR:
							if(offset+file_table[fd].open_offset>=file_table[fd].size)
											panic("fs_lseek: out of the file");
							else
											return file_table[fd].open_offset=file_table[fd].open_offset+offset;
							break;
		case SEEK_END:
							if(offset>0)
											panic("fs_lseek: Should not reach here(SEEK_END)");
							else
											return file_table[fd].open_offset=file_table[fd].size+offset;
							break;
		default: panic("fs_lseek: Should not reach here");
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
}
