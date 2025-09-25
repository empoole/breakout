#include "bricks.h"

void drawBricks(Buffer* buffer, size_t brickWidth, size_t brickHeight, Brick* bricks) {
	size_t bricksPerRow = buffer->width / brickWidth;
	for(size_t xi = 0; xi < bricksPerRow; ++xi) {
		for(size_t yi = 0; yi <= sizeof(colors); ++yi) {
			Brick* brick = &bricks[xi + yi];
			if (brick->isBroken) {
				continue;
			} else {
				size_t x = xi * brickWidth;
				size_t y = buffer->height - (brickHeight * (yi + 1));
				if (!brick->x) {
					brick->x = x;
					brick->y = y;
					brick->height = brickHeight;
					brick->width = brickWidth;
					brick->isBroken = false;
				}
				bufferDrawRect(buffer, brickHeight, brickWidth, x, y, colors[yi]);
			}
		}
	}
}
