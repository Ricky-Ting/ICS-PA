#ifndef __FS_H__
#define __FS_H__

#include "common.h"
#include <unistd.h>
#include <sys/types.h>


#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
size_t fs_filesz(int fd);

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset,size_t len);
size_t fb_write(const void * buf, size_t offset, size_t len);
#endif
