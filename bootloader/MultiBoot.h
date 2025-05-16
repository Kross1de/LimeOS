#pragma once
#include <stdint.h>
//----------------------------------------------------- defines
#define multibootTagTypeMMap			6
#define multibootMemoryAvailable		1
#define multibootMemoryReserved			2
#define multibootMemoryACPIreclaimable	3
#define multibootMemoryNVS				4
#define multibootMemoryBadRAM			5
//----------------------------------------------------- structures
struct __attribute__((packed)) multibootTag
{
	uint32_t type;
	uint32_t size;
};
struct __attribute__((packed)) multibootMMapEntry
{
	uint64_t addr;
	uint64_t len;
	uint32_t type;
	uint32_t zero;
};
struct __attribute__((packed)) multibootTagMMap
{
	uint32_t type;
	uint32_t size;
	uint32_t entrySize;
	uint32_t entryVersion;
	struct multibootMMapEntry entries[];
};
//----------------------------------------------------- declarations
void *mboot2FindTag(void *base,uint32_t type);
