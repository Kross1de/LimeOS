#include <stdint.h>
#include "Stuff/printf.h" 

struct __attribute__((packed)) gentry{
	uint16_t Limit;
	uint16_t Basel;
	uint8_t  Basem;
	uint8_t  access;
	uint8_t  gran;
	uint8_t  Baseh;
};

struct __attribute__((packed)) gdtr{
	uint16_t size;
	uint64_t offset;
};

struct gentry gdtentr[9];
struct gdtr		gdtDesc;

extern void gdtFlush(void);

void gdtSE(uint8_t index,uint16_t limit,uint32_t base,uint8_t access,uint8_t gran){
	gdtentr[index].Limit		= limit;
	gdtentr[index].Basel		= base & 0xffff;
	gdtentr[index].Basem		= (base>>16) & 0xff;
	gdtentr[index].access 		= access;
	gdtentr[index].gran			= gran;
	gdtentr[index].Baseh		= (base>>24) & 0xff;
}

void gdtInstall(void){
	gdtSE(0,0x0000,0x00000000,0x00,0x00);
	gdtSE(1,0x0000,0x00000000,0x9A,0x20);
	gdtSE(2,0x0000,0x00000000,0x92,0x00);
	gdtSE(3,0x0000,0x00000000,0xFA,0x20);
	gdtSE(4,0x0000,0x00000000,0xF2,0x00);
	
    gdtDesc = (struct gdtr) {
        .size=sizeof(struct gentry)*9-1,
        .offset=(uint64_t)&gdtentr
    };
	gdtFlush();
	
	printf("%s:%d: GDT address: 0x%x\n", __FILE__,__LINE__,(uint64_t)&gdtDesc);
}
