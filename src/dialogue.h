extern bool inDialogue;

void showDialogue(const char* string[3]);
void hideDialogue();
void drawDialogue();

void setOnDialogueHide(void (*onHide)());