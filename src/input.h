#include <tice.h>

void setInputHandler(void (*handler)(uint8_t));
void* getInputHandler();
void handleInput(uint8_t direction);