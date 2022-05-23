#define DIALOGUE_TYPE_IMPERSONAL 0
#define DIALOGUE_TYPE_PERSONAL 1

extern bool inDialogue;

void showDialogue(const char* string[3], uint8_t type);
void hideDialogue();
void drawDialogue();

void setOnDialogueHide(void (*onHide)());