#ifndef BALL
#define BALL

struct Ball {
	size_t x, y;
	size_t direction_x, direction_y;
	size_t speed;
	uint32_t color;
};

#endif // BALL