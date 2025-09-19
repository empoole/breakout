#ifndef UTILS
#define UTILS

#include <cstdio>
#include <cstdint>

void errorCallback(int error, const char* description);
uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b);

#endif // UTILS
