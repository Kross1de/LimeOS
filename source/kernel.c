#include "video/VGA.h"
#include "Stuff/printf.h"
#include "system/version.h"
#include "Stuff/CPU/GDT.h"
#include "Stuff/CPU/IDT.h"
#include "Stuff/serial.h"

void kmain(void *mboot_info) {
		vga_clear();
		vga_enable_cursor();
		printf("%s %d.%d.%d %s %s %s\n",KernelName,KernelVersionMajor,KernelVersionMinor,KernelVersionPatch,KernelBuildDate,KernelBuildTime,KernelArch);
		serialInstall();
        gdtInstall();
        idt_install();
}
