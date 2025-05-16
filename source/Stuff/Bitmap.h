#pragma once
#include <stdint.h>
#include <stdbool.h>

static inline void bitmapSet(uint8_t *bitmap,uint32_t bit){
	bitmap[bit/8]|=1<<(bit%8);
}
static inline void bitmapClear(uint8_t *bitmap,uint32_t bit){
	bitmap[bit/8]&=~(1<<(bit%8));
}
static inline bool bitmapGet(const uint8_t *bitmap,uint32_t bit){
	return bitmap[bit/8]&(1<<(bit%8));
}
