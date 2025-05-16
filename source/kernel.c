#include <stddef.h>
#include "../bootloader/MultiBoot.h"
#include "Stuff/common/assert.h"
#include "video/VGA.h"
#include "Stuff/printf.h"
#include "system/version.h"
#include "Stuff/CPU/GDT.h"
#include "Stuff/CPU/IDT.h"
#include "Stuff/MemoryManagement/PMM.h"
#include "Stuff/serial.h"
//----------------------------------------------
void *mboot2FindNext(char *current,uint32_t type){
	char *header=current;
	while((uintptr_t)header&7)header++;
	struct multibootTag *tag = (void *)header;
	while(1)
	{
		if(tag->type==0) return NULL;
		if(tag->type==type) return tag;
		
		header+=tag->size;
		while((uintptr_t)header&7)header++;
		tag = (void*)header;
	}
}

void *mboot2FindTag(void *base,uint32_t type)
{
	char *header=base;
	header+=8;
	return mboot2FindNext(header,type);
}

void kmain(void *mboot_info,uint32_t mboot_magic) {
		vga_clear();
		vga_enable_cursor();
		serialInstall();
		printf(",--.   ,--.                  ,-----.  ,---.\n");
		printf("|  |   `--',--,--,--. ,---. '  .-.  ''   .-' \n");
		printf("|  |   ,--.|        || .-. :|  | |  |`.  `-.\n");
		printf("|  '--.|  ||  |  |  |   --.'  '-'  '.-'    |\n");
		printf("`-----'`--'`--`--`--' `----' `-----' `-----'\n");
		printf("%s %d.%d.%d %s %s %s\n",KernelName,KernelVersionMajor,KernelVersionMinor,KernelVersionPatch,KernelBuildDate,KernelBuildTime,KernelArch);
		assert(mboot_magic == 0x36d76289);
        gdtInstall();
        idt_install();
        pmminstall(mboot_info);
}
