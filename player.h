#include <cstddef>
#include <cstdint>

struct Player {
	size_t x, y;
	size_t width, height;
	int8_t direction;
	int8_t speed;
	uint32_t color;
};
