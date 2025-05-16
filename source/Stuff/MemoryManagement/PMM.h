#pragma once
#include <stddef.h>

void pmminstall(void *mboot_info);
void *pmmAlloc(size_t pageCount);
void pmmFree(void *ptr,size_t pageCount);
