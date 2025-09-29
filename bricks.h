#ifndef BRICKS
#define BRICKS

#include "graphicsBuffer.h"
#include "utils.h"
#include "ball.h"

struct Brick {
	size_t x, y;
	size_t height, width;
	size_t bottom;
	bool isBroken;
};

const uint32_t colors[7] = {
	rgbToUint32(128, 0, 0), // red
	rgbToUint32(128, 74, 0), // orange
	rgbToUint32(128, 128, 0), // yellow
	rgbToUint32(0, 128, 0), // green
	rgbToUint32(0, 0, 128), // blue
	rgbToUint32(37, 0, 128), //indigo
	rgbToUint32(74, 0, 128) // violet
};

void initBricks(Brick* bricks, size_t brickHeight, size_t brickWidth, size_t bufferWidth, size_t numRows);
void drawBricks(Buffer* buffer, size_t brickWidth, size_t brickHeight, Brick* bricks);
bool detectBrickCollision(const size_t ballX, const size_t ballY, const Brick* brick);

#endif // BRICKS
