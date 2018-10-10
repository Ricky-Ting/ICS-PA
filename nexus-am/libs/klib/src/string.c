#include "klib.h"

#ifndef __ISA_NATIVE__

size_t strlen(const char *s) {
		size_t length=0;
		while(*s!='\0') {
			s++;
			length++;
		}
  return length;
}

char *strcpy(char* dst,const char* src) {
				char *s = dst;
		 		while( (*s = *src) !=0 ) {
					s++;
					src++;
				}
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
		 	/*
				strncpy funcion doesn't promise the final dst is ending with '\0'. When there is still space, it fills dst with '\0';
			*/


			char *s=dst;
		 	while (n>0 && *src!='\0') {
						*s=*src;
						s++;
						src++;
						n--;
			}
			while(n>0) {
						*s='\0';
						n--;
		 	}
			return dst;
}

char* strcat(char* dst, const char* src) {
			char *s= dst + strlen(dst);
			strcpy(s,src);
			return dst;
  
}

int strcmp(const char* s1, const char* s2) {
	while( (*s1)==(*s2)  ) {
			if( (*s1)=='\0')
					return 0;
			s1++;
			s2++;
	} 
	
	return  ((*(unsigned char *)s1) < (*(unsigned char *)s2)) ? -1 : +1; 



  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
				while(n>0) { 
						if(*s1!=*s2)
											return  ((*(unsigned char *)s1) < (*(unsigned char *)s2)) ? -1 : +1; 
						else if( *s1=='\0')
										return 0;
						n--;
						s1++;   s2++;
				}
  return 0;
}

void* memset(void* v,int c,size_t n) {
				const unsigned char uc=c;
				unsigned char *dst=v;
				for(;n>0;n--,dst++)
								*dst=uc;
				return v;
}

void* memcpy(void* out, const void* in, size_t n) {
				unsigned char *o=out;
				const unsigned char *i=in;
				for(;n>0;n--,o++,i++) 
							*o=*i;
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){

				for(;n>0;n--,s1++,s2++) 
						if(*(unsigned char *)s1!=*(unsigned char *)s2)
											return  ((*(unsigned char *)s1) < (*(unsigned char *)s2)) ? -1 : +1; 
  return 0;
}

#endif
