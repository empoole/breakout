#ifndef GRAPHICS_BUFFER
#define GRAPHICS_BUFFER

#include <cstddef>
#include <cstdint>

struct Buffer {
	size_t width, height;
	uint32_t* data;
};

struct Sprite {
	size_t height, width;
	uint8_t* data;
};

void bufferClear(Buffer* buffer, uint32_t color);
void bufferDrawRect(Buffer* buffer, size_t height, size_t width, size_t x, size_t y, uint32_t color);
void bufferDrawSprite(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color);
void bufferDrawBall(Buffer* buffer, size_t x, size_t y, uint32_t color);
//void bufferDrawCircle(Buffer* buffer, size_t radius, size_t x, size_t y, uint32_t color);


#endif // GRAPHICS_BUFFER
