#include <tice.h>

void* determineAppropriatePalette(size_t* paletteSize);
void initGfx();

void fadeOut(void);
void fadeIn(void);

void unzipScaleDrawSprite(void* compressed_sprite, uint8_t compressedW, uint8_t compressedH, uint8_t scaleFactor, int x, int y);

void defaultInputHandler(sk_key_t key);

void setRenderNiko(bool render);
void setGfxActive(bool active);
void markEndProgram();