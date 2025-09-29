#ifndef BALL
#define BALL

struct Ball {
	float x, y;
	float direction_x, direction_y;
	float speed;
	uint32_t color;
};

#endif // BALL