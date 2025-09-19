#ifndef GRAPHICS_BUFFER
#define GRAPHICS_BUFFER

#include <cstddef>
#include <cstdint>
#include "sprite.h"

struct Buffer {
	size_t width, height;
	uint32_t* data;
};

void bufferClear(Buffer* buffer, uint32_t color);
//void bufferDrawSprite(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color);
void bufferDrawRect(Buffer* buffer, size_t height, size_t width, size_t x, size_t y, uint32_t color);

#endif // GRAPHICS_BUFFER
