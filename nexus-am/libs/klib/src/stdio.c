#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define BUFF_SIZE 1<<12
char buffer[BUFF_SIZE];//数组大小可调

char buf[128];

int printf(const char *fmt, ...) {
  static char buff[BUFF_SIZE];
  va_list ap = (va_list)((char*)(&fmt) + 4);
  va_start(ap, fmt);
  int r_num = vsprintf(buff, fmt, ap);
  //printf("%s\n", buff);
  va_end(ap);
  for(char* s = buff;*s != '\0';s++){
    //printf("%c", *s);
    _putc(*s);
  }
  return r_num;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char* p = buffer;
  while(*fmt != '\0'){
    while(*fmt != '\0'){
    	//printf("hhh");
  	  //printf("%c", *fmt);
      if(*fmt == '\\'){
        *p = *fmt;
        *(p+1) = *(fmt+1);
        p += 2, fmt += 2;
      }
      else if(*fmt == '%'){
      	fmt++; 
    	  break;
  	}
      else{
        *p = *fmt;
        p++, fmt++;
      }
    }

    //修改转换行为的标签
    int width = 0;//used to record the asked width of float and int.
    switch(*fmt){
      case '-':
      case '+':
      case ' ':
        assert(0);
      case '#':
        fmt++;
        p[0] = '0';
        p[1] = 'x';
        p += 2;
        break;
      case '0':
        fmt++;
        while(*fmt <= '9' && *fmt >= '0'){
          width = width*10+(int)(*fmt-'0');
          fmt++;
        }
        break;
      default: break;
    }
    if(width == 0)//make sure there is at least one number to be output
      width++;

    if(*fmt == '.'){
      assert(0);
    }

    static char* s;
    static int d;
    static unsigned int ud;
    int i;
    static char d_c[11];
    int x_ul = 0;
    //Because they must be the first statement of their case while the first statement cannot be a declaration, so they are put here
    switch(*fmt){
      case 'c': assert(0); break;
      case 's':
        s = va_arg(ap, char*);
        int len = 0;
        while(s[len] != '\0'){
        	//printf("%c", s[len]);
          *p = s[len];
          p++, len++;
        }
        fmt++;
        break;
      case 'd':
      case 'i':
        d = va_arg(ap, int);
        //printf("hhh\n");
        if(d < 0){
          *p = '-';
          p++;
          d = -d;
        }
        for(i = 9;i >= 0;i--){
          int rem = d%10;
          d /= 10;
          if(d == 0 && rem == 0)
            break;
          d_c[i] = rem + '0';
          //printf("@%c\n", d_c[i]);
        }
        for(int j = 9-i;j < width;j++){
          *p = '0';
          p++;
        }
        for(i += 1;i < 10;i++){
          //_putc(d_c[i]);
          *p = d_c[i];
          p++;
        }
        fmt++;
        break;
      case 'o': assert(0); break;
      case 'x':
        x_ul = 'a';
      case 'p':
        if(*fmt == 'p')
          width = 8;
      case 'X':
        if(*fmt == 'X')
          x_ul = 'A';
        
        ud = va_arg(ap, unsigned int);
        //printf("hhh\n");
        for(i = 9;i >= 0;i--){
          int rem = ud%16;
          ud /= 16;
          if(ud == 0 && rem == 0)
            break;
          if(rem <= 9)
            d_c[i] = rem + '0';
          else
            d_c[i] = rem + x_ul - 10;
          //printf("@%c\n", d_c[i]);
          //_putc(d_c[i]);
        }
        for(int j = 9-i;j < width;j++){
          *p = '0';
          p++;
        }
        for(i += 1;i < 10;i++){
          *p = d_c[i];
          p++;
        }
        fmt++;
        break;
      case 'u': assert(0); break;
      case 'f':
      case 'F': assert(0); break;
      case 'e':
      case 'E': assert(0); break;
      case 'a':
      case 'A': assert(0); break;
      case 'g':
      case 'G': assert(0); break;
      case 'n': assert(0); break;
    }
  } 
  *p = '\0';
  for(char* p_ = buffer;p_ != p;p_++, out++){
    *out = *p_;
  }
  *out = '\0';

  return (int)(p-buffer);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap =  (va_list)((char*)(&fmt) + 4);
  va_start(ap, fmt);
  int r_num = vsprintf(out, fmt, ap);
  //printf("%s\n", out);
  va_end(ap);
  return r_num;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  char buff[BUFF_SIZE];
  va_list ap = (va_list)((char*)(&fmt) + 4);
  va_start(ap, fmt);
  vsprintf(buff, fmt, ap);
  //printf("%s\n", buff);
  va_end(ap);
  char* s = buff;
  int i;
  for(i = 0;i < n-1 && s[i] != '\0';i++){
    out[i] = s[i];
  }
  out[i] = '\0';
  return i;
}

#endif
