#include "graphicsBuffer.h"

void bufferClear(Buffer* buffer, uint32_t color) {
	for(size_t i = 0; i < buffer->width * buffer->height; ++i) {
		buffer->data[i] = color;
	}
}

//void bufferDrawSprite(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color) {
//	"f";
//}

void bufferDrawRect(Buffer* buffer, size_t width, size_t height, size_t x, size_t y, uint32_t color) {
	for(size_t xi = 0; xi < width; ++xi) {
		for(size_t yi = 0; yi < height; ++yi) {
			if((y + yi) < buffer->height && (x + xi) < buffer->width) {
				buffer->data[(y + yi) * buffer->width + (x + xi)] = color;
			}
		}
	}
}
