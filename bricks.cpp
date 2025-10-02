#include "bricks.h"
#include "ball.h"

void initBricks(Brick* bricks, size_t brickHeight, size_t brickWidth, size_t bufferWidth, size_t numRows = 7) {
	size_t bricksPerRow = bufferWidth / brickWidth;
	for(int i = 0; i < NUM_BRICKS; ++i) {
		Brick* brick = &bricks[i];
		brick->height = brickHeight;
		brick->width = brickWidth;
		brick->isBroken = false;
	}
}

void drawBricks(Buffer* buffer, size_t brickWidth, size_t brickHeight, Brick* bricks) {
	size_t bricksPerRow = buffer->width / brickWidth;
	size_t row = 0;
	size_t col = 0;
	for (int i = 0; i < NUM_BRICKS; ++i) {
		Brick* brick = &bricks[i];
		uint32_t color = colors[row];
		if (brick->isBroken) color = rgbToUint32(0, 0, 0);
		size_t x = col * brickWidth;
		size_t y = buffer->height - (brickHeight * (row + 1));
		// initialize brick coordiantes
		if(!brick->x) {
			brick->x = x;
			brick->y = y;
			brick->bottom = y + brickHeight;
		}
		bufferDrawRect(buffer, brickHeight, brickWidth, x, y, color);
		col++;
		if(col == bricksPerRow) {
			row++;
			col = 0;
		}
	}
}

bool detectBrickCollision(const size_t ballX, const size_t ballY, const Brick* brick) {
	if (ballX + 6 >= brick->x && ballX <= brick->x + brick->width &&
		ballY+ 6 >= brick->y && ballY <= brick->y + brick->height &&
		!brick->isBroken) {
		return true;
	}
	return false;
}
