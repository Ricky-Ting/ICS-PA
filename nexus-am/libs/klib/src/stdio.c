#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

#define true 1
#define false 0
#define bool char

void parse_int(int a, char* str);

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
				while(*fmt!='\0') {
			/*		char myflags='\0';
					char width[10];
					char precision[10];
					char length='\0';
					char specifier='\0';
*/
					if(*fmt!='%') {
						*out=*fmt;
						out++;
						fmt++;
					}
					else {
								fmt++;
/*						myflags=length=width[0]=precision[0]='\0';
						// get_flags  
						while(*fmt==' ') {
							myflags=' ';
							fmt++;
						}	
						switch(*fmt) {
							case '-': myflags='-'; fmt++; break;
							case '+': myflags='+'; fmt++; break;
							case '#': myflags='#'; fmt++; break;
							case '0': myflags='0'; fmt++; break;
							default: break;
						}

						// get_width 
						while((*fmt)>='0' && (*fmt)<='9') {
							char tmp[2]; tmp[0]=*fmt; tmp[1]='\0';
							strcat(width,tmp);
							fmt++;
						}
						if(*fmt=='*') {
							width[0]='*'; width[1]='\0';
							fmt++;
						}

						// get_precision
						if(*fmt=='.') {
									while((*fmt)>='0' && (*fmt)<='9') {
											char tmp[2]; tmp[0]=*fmt; tmp[1]='\0';
											strcat(precision,tmp);
											fmt++;
									}
									if(*fmt=='*') {
											precision[0]='*'; precision[1]='\0';
											fmt++;
									}
						}
							
						// get_length
						switch(*fmt) {
							case 'h': length='h'; fmt++; break;
							case 'l': length='l'; fmt++; break;
							case 'L': length='L'; fmt++; break;
							default: break;
						}
						

						
*/
						int a; char *str=NULL;
						switch(*fmt) {
							case 'd':  
								a=va_arg(ap,int);
								parse_int(a,str);
								strcat(out,str);
								out+=strlen(str);
								break;
							case 's': 
								str=va_arg(ap,char *);
								strcat(out,str);
								out+=strlen(str);
								break;
							default: return -1;  break;
						}
					}
				}
  return strlen(out);
}

		
void parse_int(int a,char* str){
				char b;
				bool isneg=false; bool isnegmax=false;
				char str2[20];
				if(a==0)  {
						str[0]='0'; str[1]='\0';
						return;
				} 
				else if(a <0) {
						isneg=true;
						int b=-a;
						if(b==a){
								isnegmax=true;
								a=a+1;
						} 
						a=-a;
				}
				int i=0;
				while(a!=0) {
						b='0'+a%10;
						str2[i]=b;
						a=a/10;
						i++;
				}
				if(isneg) {
						if(isnegmax) {
							str2[i-1]+=1;				
						}
						str2[i]='-';
						i++;
					}
				
				str2[i]='\0';
				for(int j=0;j<i;j++)
							str[j]=str2[i-j-1];
				str[i]='\0';
				return ;
}


int sprintf(char *out, const char *fmt, ...) {
		va_list ap;
		va_start(ap,fmt);
		return vsprintf(out,fmt,ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
