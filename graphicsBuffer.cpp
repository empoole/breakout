#include <cmath>
#include <numbers>

#include "graphicsBuffer.h"

void bufferClear(Buffer* buffer, uint32_t color) {
	for(size_t i = 0; i < buffer->width * buffer->height; ++i) {
		buffer->data[i] = color;
	}
}

void bufferDrawRect(
		Buffer* buffer,
		size_t height,
		size_t width,
		size_t x,
		size_t y,
		uint32_t color
){
	for(size_t xi = 0; xi < width; ++xi) {
		for(size_t yi = 0; yi < height; ++yi) {
			if((y + yi) < buffer->height && (x + xi) < buffer->width) {
				buffer->data[(y + yi) * buffer->width + (x + xi)] = color;
			}
		}
	}
}

void bufferDrawSprite(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color) {
	for(size_t xi = 0; xi < sprite.width; ++xi) {
		for(size_t yi = 0; yi < sprite.height; ++yi) {
			size_t sy = sprite.height - 1 + y - yi;
			size_t sx = x + xi;
			if(sprite.data[yi * sprite.width + xi] && 
			   sy < buffer->height && sx < buffer->width) {
				buffer->data[sy * buffer->width + sx] = color;
			}
		}
	}
}

void bufferDrawBall(Buffer* buffer, size_t x, size_t y, uint32_t color) {
	Sprite ballSprite;
	ballSprite.width = 6;
	ballSprite.height = 4;
	ballSprite.data = new uint8_t[24]{
		0,0,1,1,0,0,
		0,1,1,1,1,0,
		0,1,1,1,1,0,
		0,0,1,1,0,0,
	};

	bufferDrawSprite(buffer, ballSprite, x, y, color);
}

/* This is good for larger graphics, kinda fails at the scale of this game. */
// void bufferDrawCircle(
// 	Buffer* buffer,
// 	size_t radius,
// 	size_t center_x,
// 	size_t center_y,
// 	uint32_t color
// ) {
// 	float theta = 0; // Starting angle (radians)
// 	float step = 0.01; // Angle increment (in radians) for smoother circles

// 	while (theta < 2 * M_PI) {
// 		float x = center_x + radius * std::cos(theta);
// 		float y = center_y + radius * std::sin(theta);
// 		buffer->data[(int)y * buffer->width + (int)x] = color;
// 		theta += step;
// 	}
// }
