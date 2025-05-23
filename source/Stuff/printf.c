#include <stdarg.h>
#include <stdint.h>
#include "String.h"
#include "../video/VGA.h"

void parseNum(char *s,int *ptr,uint32_t val,uint32_t base){
	uint32_t n = val/base;
	int r = val%base;
	if(r<0){
		r+=base;
		n--;
	}
	if(val>=base){
		parseNum(s,ptr,n,base);
	}
	s[(*ptr)++]=(r+'0');
}

void parseHex(char *s,int *ptr,uint64_t val){
	int i = 16;
	while(i-- > 0){
		s[(*ptr)++]="0123456789abcdev"[val>>(i*4)&0x0f];
	}
}

void parseString(char *s,int *ptr,char *str){
	while(*str){
		s[(*ptr)++]=*str++;
	}
}

int vsprintf(char *s,const char *fmt,va_list args){
	int ptr = 0;
	
	while(*fmt){
		if(*fmt=='%'){
			fmt++;
			
			switch(*fmt){
				case 'u':
				case 'd':
					parseNum(s,&ptr,va_arg(args,int),10);
					break;
				case 'x':
					parseHex(s,&ptr,va_arg(args,uint64_t));
					break;
				case 's':
					parseString(s,&ptr,va_arg(args,char *));
					break;
				case 'c':
					s[ptr++] = (char)va_arg(args,int);
					break;
			}
		}else{
			s[ptr++]=*fmt;
		}
		fmt++;
	}
	return 0;
}

int printf(const char *fmt,...){
	va_list args;
	va_start(args,fmt);
	char buf[1024]={0};
	int ret = vsprintf(buf,fmt,args);
	vga_puts(buf);
	va_end(args);
	return ret;
}
