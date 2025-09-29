#ifndef PLAYER
#define PLAYER

#include <cstddef>
#include <cstdint>

struct Player {
	float x;
	size_t y;
	size_t width, height;
	float direction;
	float speed;
	uint32_t color;
};

#endif // PLAYER