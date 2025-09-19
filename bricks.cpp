#include "bricks.h"

void drawBricks(Buffer* buffer, size_t width, size_t height, size_t brickWidth, size_t brickHeight) {
	size_t bricksPerRow = buffer->width / brickWidth;
	for(size_t xi = 0; xi < bricksPerRow; ++xi) {
		for(size_t yi = 0; yi <= sizeof(colors); ++yi) {
			size_t x = xi * brickWidth;
			size_t y = buffer->height - (brickHeight * (yi + 1));
			bufferDrawRect(buffer, brickHeight, brickWidth, x, y, colors[yi]);
		}
	}
}
