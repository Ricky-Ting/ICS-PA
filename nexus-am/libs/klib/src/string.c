#include "klib.h"

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#include "/usr/include/stdio.h"

#ifdef DEBUG
#define ASSERT_LEN(len, name) \
if(len >= 0x7fffffff){ \
  printf("\33[1;31m Error: len in %s is too long.\n", name); \
  assert(0); \
}
#else
#define ASSERT_LEN(len, name)
#endif

size_t strlen(const char *s) {
  int len = 0;
  while(s[len] != '\0'){
    len++;
    ASSERT_LEN(len, "strlen");
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  int len = 0;
  *dst = *src;
  do{
    len++;
    dst[len] = src[len];
    ASSERT_LEN(len, "strcpy");
  } while(src[len] != '\0');
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  int i;
  for(i = 0;i < n;i++){
    if(src[i] == '\0')
      break;
    dst[i] = src[i];
    ASSERT_LEN(i, "strncpy");
  }
  for(;i < n;i++){
    dst[i] = '\0';
    ASSERT_LEN(i, "strncpy");
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  char* end = dst;
  while(*(end) != '\0'){
    end++;
    ASSERT_LEN(end-dst, "strcat");
  }
  int len = 0;
  *end = *src;
  do{
    len++;
    end[len] = src[len];
    ASSERT_LEN(len, "strcat");
  } while(src[len] != '\0');
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  int len = 0;
  while(s1[len] != '\0' ||  s2[len] != '\0'){
    static signed char diff;
    diff = s1[len] - s2[len];
    if(diff > 0)
      return 1;
    if(diff < 0)
      return -1;
    len++;
    ASSERT_LEN(len, "strcmp");
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  for(int i = 0;i < n && (s1[i] != '\0' ||  s2[i] != '\0');i++){
    static signed char diff;
    diff = s1[i] - s2[i];
    if(diff > 0)
      return 1;
    if(diff < 0)
      return -1;
    ASSERT_LEN(i, "strncmp");
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  char* v_ = (char*)v;
  unsigned char c_ = (unsigned char)c;
  for(size_t i = 0;i < n;i++){
    v_[i] = c_;
    ASSERT_LEN(i, "memset");
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  char* out_ = (char*)out;
  char* in_ = (char*)in;
  for(size_t i = 0;i < n;i++){
    out_[i] = in_[i];
    ASSERT_LEN(i, "memcpy");
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* s1_ = (char*)s1;
  char* s2_ = (char*)s2;
  for(size_t i = 0;i < n && (s1_[i] != '\0' || s2_[i] != '\0');i++){
    static signed char diff;
    diff = s1_[i] - s2_[i];
    if(diff > 0)
      return 1;
    if(diff < 0)
      return -1;
    ASSERT_LEN(i, "memcmp");
  }
  return 0;
}

#endif
