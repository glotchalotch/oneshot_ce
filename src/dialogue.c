#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "color.h"
#include "dialogue.h"

#define height gfx_lcdHeight / 3

gfx_sprite_t* behindDialogue;
gfx_sprite_t* behindDialogue2;
gfx_sprite_t* portrait;
const char* curString1;
const char* curString2;
const char* curString3;
bool inDialogue = false;

uint8_t dialogueType;
unsigned int strWidth1;
unsigned int strWidth2;
unsigned int strWidth3;
unsigned int *strWidthMax;

void* onDialogueHidePtr;

void drawDialogue() {
    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        gfx_SetColor(COLOR_BLACK);
        gfx_FillRectangle(0, gfx_lcdHeight - height, gfx_lcdWidth, height);
        gfx_SetColor(COLOR_TRANSPARENT_GREEN);
        gfx_PrintStringXY(curString1, 85, gfx_lcdHeight - height + 10);
        gfx_PrintStringXY(curString2, 85, gfx_lcdHeight - height + 30);
        gfx_PrintStringXY(curString3, 85, gfx_lcdHeight - height + 50);
        gfx_TransparentSprite(portrait, 0, gfx_lcdHeight - height);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        gfx_SetTextBGColor(COLOR_BLACK);
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(curString1, (gfx_lcdWidth/2) - (strWidth1/2), gfx_lcdHeight * .5 - 20);
        gfx_PrintStringXY(curString2, (gfx_lcdWidth/2) - (strWidth2/2), gfx_lcdHeight * .5);
        gfx_PrintStringXY(curString3, (gfx_lcdWidth/2) - (strWidth3/2), gfx_lcdHeight * .5 + 20);
    }

}

void showDialogue(const char* string[3], uint8_t type) {
    dialogueType = type;
    curString1 = string[0];
    curString2 = string[1];
    curString3 = string[2];

    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        portrait = gfx_MallocSprite(80, 80);
        gfx_sprite_t *niko_portrait_neutral = gfx_MallocSprite(niko_portrait_neutral_width, niko_portrait_neutral_height);
        zx7_Decompress(niko_portrait_neutral, niko_portrait_neutral_compressed);
        gfx_ScaleSprite(niko_portrait_neutral, portrait);
        free(niko_portrait_neutral);

        // gfx_sprite_t width and height are uint8, so i have to have two sprites w/ one as 255 width
        behindDialogue = gfx_MallocSprite(255, height);
        behindDialogue2 = gfx_MallocSprite(gfx_lcdWidth - 255, height);
        gfx_GetSprite(behindDialogue, 0, gfx_lcdHeight - height);
        gfx_GetSprite(behindDialogue2, 255, gfx_lcdHeight - height);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        strWidth1 = gfx_GetStringWidth(curString1);
        strWidth2 = gfx_GetStringWidth(curString2);
        strWidth3 = gfx_GetStringWidth(curString3);
        if(strWidth1 > strWidth2) strWidthMax = &strWidth1;
        else strWidthMax = &strWidth2;
        if(strWidth3 > *strWidthMax) strWidthMax = &strWidth3;
        behindDialogue = gfx_MallocSprite(*strWidthMax, 60);
        gfx_GetSprite(behindDialogue, (gfx_lcdWidth / 2) - (*strWidthMax / 2), gfx_lcdHeight * .5 - 20);
    }
    

    drawDialogue();

    inDialogue = true;
}

void onDialogueHide(void (*onHide)()) {
    onHide();
}

void hideDialogue() {
    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        gfx_TransparentSprite(behindDialogue, 0, gfx_lcdHeight - height);
        gfx_TransparentSprite(behindDialogue2, 255, gfx_lcdHeight - height);

        free(behindDialogue2);
        free(portrait);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        gfx_TransparentSprite(behindDialogue, (gfx_lcdWidth / 2) - (*strWidthMax / 2), gfx_lcdHeight * .5 - 20);
    }
    free(behindDialogue);

    inDialogue = false;

    if(onDialogueHidePtr != NULL) onDialogueHide(onDialogueHidePtr);
}

void setOnDialogueHide(void (*onHide)()) {
    onDialogueHidePtr = onHide;
}