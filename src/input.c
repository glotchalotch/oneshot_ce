#include "input.h"

void (*currentInputHandler)(sk_key_t);
void setInputHandler(void (*handler)(sk_key_t)) {
    currentInputHandler = handler;
}
void* getInputHandler() {
    return currentInputHandler;
}

void handleInput(sk_key_t key) {
    currentInputHandler(key);
}
