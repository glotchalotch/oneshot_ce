#include <tice.h>

void drawFgTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y);
void drawBgTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y);
void drawOutlineTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y);
void expandQuadrant(uint8_t q);