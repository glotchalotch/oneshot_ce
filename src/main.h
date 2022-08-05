//TODO all of these bools really should not exist
extern bool renderNiko;
extern bool gfxActive;
extern bool endProgram;
void initGfx();

void unzipScaleDrawSprite(void* compressed_sprite, uint8_t compressedW, uint8_t compressedH, uint8_t scaleFactor, int x, int y);