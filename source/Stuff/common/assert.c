#include <stdint.h>
#include "../printf.h"
//---------------------------------------------------------
extern void archFatal(void);
//---------------------------------------------------------
void __assert_failed(const char *file,uint32_t line,const char *func,const char *cond){
	printf("%s:%d: (%s) Assertion failed: %s\n",file,line,func,cond);
	archFatal();
}
