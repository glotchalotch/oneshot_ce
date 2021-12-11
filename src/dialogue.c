#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"

#define height gfx_lcdHeight / 3

gfx_sprite_t* behindDialogue;
gfx_sprite_t* behindDialogue2;
gfx_sprite_t* portrait;
const char* curString1;
const char* curString2;
const char* curString3;
bool inDialogue = false;

void* onDialogueHidePtr;

void drawDialogue() {
    gfx_SetColor(1);
    gfx_FillRectangle(0, gfx_lcdHeight - height, gfx_lcdWidth, height);
    gfx_SetColor(0);
    gfx_PrintStringXY(curString1, 85, gfx_lcdHeight - height + 10);
    gfx_PrintStringXY(curString2, 85, gfx_lcdHeight - height + 30);
    gfx_PrintStringXY(curString3, 85, gfx_lcdHeight - height + 50);
    gfx_TransparentSprite(portrait, 0, gfx_lcdHeight - height);
}

void showDialogue(const char* string[3]) {
    curString1 = string[0];
    curString2 = string[1];
    curString3 = string[2];

    portrait = gfx_MallocSprite(80, 80);
    gfx_sprite_t* niko_portrait_neutral = gfx_MallocSprite(niko_portrait_neutral_width, niko_portrait_neutral_height);
    zx7_Decompress(niko_portrait_neutral, niko_portrait_neutral_compressed);
    gfx_ScaleSprite(niko_portrait_neutral, portrait);
    free(niko_portrait_neutral);

    //gfx_sprite_t width and height are uint8, so i have to have two sprites w/ one as 255 width
    behindDialogue = gfx_MallocSprite(255, height);
    behindDialogue2 = gfx_MallocSprite(gfx_lcdWidth - 255, height);
    gfx_GetSprite(behindDialogue, 0, gfx_lcdHeight - height);
    gfx_GetSprite(behindDialogue2, 255, gfx_lcdHeight - height);

    drawDialogue();

    inDialogue = true;
}

void onDialogueHide(void (*onHide)()) {
    onHide();
}

void hideDialogue() {
    gfx_TransparentSprite(behindDialogue, 0, gfx_lcdHeight - height);
    gfx_TransparentSprite(behindDialogue2, 255, gfx_lcdHeight - height);
    
    free(behindDialogue);
    free(behindDialogue2);
    free(portrait);
    inDialogue = false;

    if(onDialogueHidePtr != NULL) onDialogueHide(onDialogueHidePtr);
}

void setOnDialogueHide(void (*onHide)()) {
    onDialogueHidePtr = onHide;
}