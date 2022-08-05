#include <tice.h>

#define DIALOGUE_TYPE_IMPERSONAL 0
#define DIALOGUE_TYPE_PERSONAL 1

extern bool inDialogue;

typedef struct colored_text {
    uint8_t strIndex;
    uint8_t strPos;
    uint8_t length;
    uint8_t color;
} colored_text_t;

void setColoredText(uint8_t index, colored_text_t* textStruct);
void showDialogue(const char* string[3], uint8_t type);
void hideDialogue();
void drawDialogue();

void setOnDialogueHide(void (*onHide)());