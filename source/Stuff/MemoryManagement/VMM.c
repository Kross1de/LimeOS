#include "VMM.h"

uint64_t pml4[512] __attribute__((aligned(Pagesize)));
uint64_t pdp[512] __attribute__((aligned(Pagesize)));
uint64_t pd[512] __attribute__((aligned(Pagesize)));
uint64_t first_pt[512] __attribute__((aligned(Pagesize)));