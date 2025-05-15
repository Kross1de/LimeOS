#include "video/VGA.h"
#include "Stuff/printf.h"
#include "system/version.h"
#include "GDT.h"

void kmain(void *mboot_info) {
		vga_clear();
		vga_enable_cursor();
		printf("%s %d.%d.%d %s %s %s\n",KernelName,KernelVersionMajor,KernelVersionMinor,KernelVersionPatch,KernelBuildDate,KernelBuildTime,KernelArch);
        gdtInstall();
}
