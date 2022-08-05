#include <tice.h>

void setInputHandler(void (*handler)(sk_key_t));
void* getInputHandler();
void handleInput(sk_key_t key);