#pragma once
#include <stdint.h>

#define Pagesize 4096

#define DIV_CEILING(x,y)(x+(y-1))/y
#define ALIGN_UP(x,y)DIV_CEILING(x,y)*y
#define ALIGN_DOWN(x,y)(x/y)*y
