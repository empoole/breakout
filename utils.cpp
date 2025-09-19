#include "utils.h"

void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error %s\n", description);
}

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b) {
	return (r << 24) | (g << 16) | (b << 8) | 255;
}
