#include <graphx.h>
#include <compression.h>
#include <debug.h>

#include "gfx/gfx.h"
#include "color.h"
#include "dialogue.h"
#include "input.h"
#include "main.h"

#define height GFX_LCD_HEIGHT / 3

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

colored_text_t coloredTexts[4];
unsigned int colorX[4] = {0, 0, 0, 0};
unsigned int colorY[4] = {0, 0, 0, 0};

void* onDialogueHidePtr;

void setColoredText(uint8_t index, colored_text_t* textStruct) {
    coloredTexts[index] = *textStruct;
}

void drawDialogue() {
    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        gfx_SetColor(COLOR_BLACK);
        gfx_FillRectangle(0, GFX_LCD_HEIGHT - height, GFX_LCD_WIDTH, height);
        gfx_SetColor(COLOR_TRANSPARENT_GREEN);
        gfx_PrintStringXY(curString1, 85, GFX_LCD_HEIGHT - height + 10);
        gfx_PrintStringXY(curString2, 85, GFX_LCD_HEIGHT - height + 30);
        gfx_PrintStringXY(curString3, 85, GFX_LCD_HEIGHT - height + 50);
        gfx_TransparentSprite(portrait, 0, GFX_LCD_HEIGHT - height);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        gfx_SetTextBGColor(COLOR_BLACK);
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(curString1, (GFX_LCD_WIDTH/2) - (strWidth1/2), GFX_LCD_HEIGHT * .5 - 20);
        gfx_PrintStringXY(curString2, (GFX_LCD_WIDTH/2) - (strWidth2/2), GFX_LCD_HEIGHT * .5);
        gfx_PrintStringXY(curString3, (GFX_LCD_WIDTH/2) - (strWidth3/2), GFX_LCD_HEIGHT * .5 + 20);
        for(int i = 0; i < 4; i++) {
            if(coloredTexts[i].length > 0) {
                // TODO strings probably shouldn't be calculated every frame but it doesn't seem to affect performance or anything
                char *str = malloc(sizeof(char) * (coloredTexts[i].length + 1));
                const char *baseStr = (coloredTexts[i].strIndex == 0) ? curString1 : ((coloredTexts[i].strIndex == 1) ? curString2 : curString3);
                for (int j = 0; j < coloredTexts[i].length; j++)
                {
                    str[j] = baseStr[coloredTexts[i].strPos + j];
                }
                str[coloredTexts[i].length] = '\0'; // REMINDER TO SELF: STRINGS ARE NULL TERMINATED!!
                if(colorX[i] == 0) {
                    char *precedingStr = malloc(sizeof(char) * (coloredTexts[i].strPos + 1));
                    for (int j = 0; j < coloredTexts[i].strPos; j++)
                    {
                        precedingStr[j] = baseStr[j];
                    }
                    precedingStr[coloredTexts[i].strPos] = '\0'; // AGAIN!! NULL TERMINATE WHEN MANUALLY COPYING STRINGS!! IT CAUSED ANOTHER BUG!!
                    colorX[i] = (GFX_LCD_WIDTH / 2) - (gfx_GetStringWidth(baseStr) / 2) + gfx_GetStringWidth(precedingStr);
                    colorY[i] = ((int8_t)coloredTexts[i].strIndex - 1) * 20;
                    free(precedingStr);
                }
                gfx_SetTextFGColor(coloredTexts[i].color);
                gfx_PrintStringXY(str, colorX[i], GFX_LCD_HEIGHT * .5 + colorY[i]);
                free(str);
            }
        }
    }

}

bool screenDimmed = false;
bool screenShouldUndim = false;

void showDialogue(const char* string[3], uint8_t type, bool undimScreenAtEnd) {
    dialogueType = type;
    curString1 = string[0];
    curString2 = string[1];
    curString3 = string[2];

    screenShouldUndim = undimScreenAtEnd;

    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        portrait = gfx_MallocSprite(80, 80);
        gfx_sprite_t *niko_portrait_neutral = gfx_MallocSprite(niko_portrait_neutral_width, niko_portrait_neutral_height);
        zx7_Decompress(niko_portrait_neutral, niko_portrait_neutral_compressed);
        gfx_ScaleSprite(niko_portrait_neutral, portrait);
        free(niko_portrait_neutral);

        // gfx_sprite_t width and height are uint8, so i have to have two sprites w/ one as 255 width
        behindDialogue = gfx_MallocSprite(255, height);
        behindDialogue2 = gfx_MallocSprite(GFX_LCD_WIDTH - 255, height);
        gfx_GetSprite(behindDialogue, 0, GFX_LCD_HEIGHT - height);
        gfx_GetSprite(behindDialogue2, 255, GFX_LCD_HEIGHT - height);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        strWidth1 = gfx_GetStringWidth(curString1);
        strWidth2 = gfx_GetStringWidth(curString2);
        strWidth3 = gfx_GetStringWidth(curString3);
        if(strWidth1 > strWidth2) strWidthMax = &strWidth1;
        else strWidthMax = &strWidth2;
        if(strWidth3 > *strWidthMax) strWidthMax = &strWidth3;
        if(*strWidthMax > 255) {
            behindDialogue = gfx_MallocSprite(255, 60);
            behindDialogue2 = gfx_MallocSprite(*strWidthMax - 255, 60);
            gfx_GetSprite(behindDialogue, (GFX_LCD_WIDTH / 2) - (*strWidthMax / 2), GFX_LCD_HEIGHT * .5 - 20);
            gfx_GetSprite(behindDialogue2, (GFX_LCD_WIDTH / 2) - (*strWidthMax / 2) + 255, GFX_LCD_HEIGHT * .5 - 20);
        } else {
            behindDialogue = gfx_MallocSprite(*strWidthMax, 60);
            gfx_GetSprite(behindDialogue, (GFX_LCD_WIDTH / 2) - (*strWidthMax / 2), GFX_LCD_HEIGHT * .5 - 20);
        }

        if(!screenDimmed) {
            for(int i = 7; i < 256; i++) {
                gfx_palette[i] = gfx_Darken(gfx_palette[i], 100);
            }
            screenDimmed = true;
        }
        
    }
    

    drawDialogue();

    setInputHandler(dialogue_inputHandler);

    inDialogue = true;
}

void onDialogueHide(void (*onHide)()) {
    onHide();
}

void hideDialogue() {
    colored_text_t nullColoredText = {0, 0, 0, 0};
    coloredTexts[0] = nullColoredText;
    coloredTexts[1] = nullColoredText;
    coloredTexts[2] = nullColoredText;
    coloredTexts[3] = nullColoredText;

    colorX[0] = 0;
    colorX[1] = 0;
    colorX[2] = 0;
    colorX[3] = 0;
    colorY[0] = 0;
    colorY[1] = 0;
    colorY[2] = 0;
    colorY[3] = 0;

    if(dialogueType == DIALOGUE_TYPE_PERSONAL) {
        gfx_TransparentSprite(behindDialogue, 0, GFX_LCD_HEIGHT - height);
        gfx_TransparentSprite(behindDialogue2, 255, GFX_LCD_HEIGHT - height);

        free(behindDialogue2);
        free(portrait);
    } else if(dialogueType == DIALOGUE_TYPE_IMPERSONAL) {
        if(screenDimmed && screenShouldUndim) {
            size_t size;
            gfx_SetPalette(determineAppropriatePalette(&size), size, 0);
            screenDimmed = false;
        }
        gfx_TransparentSprite(behindDialogue, (GFX_LCD_WIDTH / 2) - (*strWidthMax / 2), GFX_LCD_HEIGHT * .5 - 20);
        if(*strWidthMax > 255) {
            gfx_TransparentSprite(behindDialogue2, (GFX_LCD_WIDTH / 2) - (*strWidthMax / 2) + 255, GFX_LCD_HEIGHT * .5 - 20);
            free(behindDialogue2);
        }
    }
    free(behindDialogue);

    inDialogue = false;

    if(onDialogueHidePtr != NULL) onDialogueHide(onDialogueHidePtr);
}

void setOnDialogueHide(void (*onHide)()) {
    onDialogueHidePtr = onHide;
}

void dialogue_inputHandler(sk_key_t key) {
    if(key == sk_Enter) {
        setInputHandler(defaultInputHandler);
        hideDialogue();
    }
}