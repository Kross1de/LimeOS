 #include "../../../bootloader/MultiBoot.h"
 #include "VMM.h"
 #include "../String.h"
 #include "../printf.h"
 #include "../Bitmap.h"
 #include <stddef.h>
 #include <stdint.h>
 //------------------------------------------- variables
 uint8_t *pmmBitmap = NULL;
 uint64_t pmmLastPage = 0;
 uint64_t pmmUsedPages = 0;
 uint64_t pmmPageCount = 0;
 uint64_t pmmUsableMem = 0;
 uint64_t pmmBitmapSize = 0;
 //------------------------------------------- functions
 void pmminstall(void *mboot_info){
	 extern void *end;
	 uintptr_t highestAddress = 0;
	 
	 struct multibootTagMMap *mmap = mboot2FindTag(mboot_info,multibootTagTypeMMap);
	 struct multibootMMapEntry *mmmt = NULL;
	 
	 for(uint32_t i=0;i<(mmap->size-sizeof(struct multibootTagMMap))/mmap->entrySize;i++)
	 {
		 mmmt = &mmap->entries[i];
		 
		 if(mmmt->type==multibootMemoryAvailable){
			 dprintf("%s:%d: Memory segment: addr=0x%x, len=0x%x, type=%u\n",__FILE__,__LINE__,mmmt->addr,mmmt->len,mmmt->type);
			 highestAddress = mmmt->addr+mmmt->len;
		 }
	 }
	 
	 pmmBitmap = (uint8_t *)&end;
	 pmmPageCount = highestAddress/Pagesize;
	 uint64_t bitmapSize = ALIGN_UP(pmmPageCount/8,Pagesize);
	 memset(pmmBitmap,bitmapSize,0xff);
	 
	 for(uint32_t i=0;i<(mmap->size-sizeof(struct multibootTagMMap))/mmap->entrySize;i++){
		 mmmt = &mmap->entries[i];
		 
		 if(mmmt->type==multibootMemoryAvailable){
			 for(uint64_t o=0;o<mmmt->len;o+=Pagesize){
				 bitmapClear(pmmBitmap,(mmmt->addr+i)/Pagesize);
			 }
		 }
	 }
	 
	 printf("%s:%d: initialized allocator at 0x%x\n",__FILE__,__LINE__,(uint64_t)pmmBitmap);
	 printf("%s:%d: usable memory: %dKilobytes\n",__FILE__,__LINE__,pmmPageCount*4);
 }
 //------------------------------------------- 
 uint64_t pmmFindPages(uint64_t pageCount) {
	 uint64_t pages = 0;
	 uint64_t firstPage = pmmLastPage;
	 
	 while(pages<pageCount){
		 if(pmmLastPage>=pmmPageCount)
			return 0;
		 
		 if(!bitmapGet(pmmBitmap,pmmLastPage+pages)){
			 pages++;
			 if(pages==pageCount){
				 for(uint64_t i=0;i<pages;i++){
					 bitmapSet(pmmBitmap,firstPage+i);
				 }
				 
				 pmmLastPage+=pages;
				 return firstPage;
			 }
		 }else{
			 pmmLastPage+=!pages?1:pages;
			 firstPage=pmmLastPage;
			 pages=0;
		 }
	 }
	 return 0;
 }
 //-------------------------------------------
 void *pmmAlloc(size_t pageCount){
	 uint64_t pages = pmmFindPages(pageCount);
	 
	 if(!pages){
		 pmmLastPage=0;
		 pages=pmmFindPages(pageCount);
		 
		 if(!pages){
			 dprintf("%s:%d: \033[33mwarning:\033[0m out of memory\n",__FILE__,__LINE__);
			 return NULL;
		 }
	 }
	 
	 uint64_t physAddr=pages*Pagesize;
	 
	 return (void*)(physAddr);
 }
 //------------------------------------------- 
 void pmmFree(void *ptr,size_t pageCount){
	 uint64_t page=(uint64_t)ptr/Pagesize;
	 
	 for(uint64_t i=0;i<pageCount;i++)
		bitmapClear(pmmBitmap,page+i);
 }
