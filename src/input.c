#include "input.h"

void (*currentInputHandler)(uint8_t);
void setInputHandler(void (*handler)(uint8_t)) {
    currentInputHandler = handler;
}
void* getInputHandler() {
    return currentInputHandler;
}

void handleInput(uint8_t direction) {
    currentInputHandler(direction);
}
