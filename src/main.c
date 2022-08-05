#include <tice.h>
#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "dialogue.h"
#include "collision.h"
#include "room_start.h"
#include "inventory.h"
#include "color.h"

enum direction {Down, Up, Left, Right};
bool renderNiko = true;
bool gfxActive = true;
bool endProgram = false;

void initGfx() {
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(COLOR_TRANSPARENT_GREEN);
    gfx_SetTextScale(1, 2);
    gfx_SetTextFGColor(COLOR_WHITE);
}

void unzipScaleDrawSprite(void* compressed_sprite, uint8_t compressedW, uint8_t compressedH, uint8_t scaleFactor, int x, int y) {
    gfx_sprite_t* sprite = gfx_MallocSprite(compressedW * scaleFactor, compressedH * scaleFactor);
    gfx_sprite_t* spriteUncompressed = gfx_MallocSprite(compressedW, compressedH);
    zx7_Decompress(spriteUncompressed, compressed_sprite);
    gfx_ScaleSprite(spriteUncompressed, sprite);
    gfx_Sprite(sprite, x, y);
    free(sprite);
    free(spriteUncompressed);
}

int main() {
    initGfx();    
        //dbg_SetBreakpoint(&inventory_toggle);

    gfx_sprite_t *niko_down_scaled = gfx_MallocSprite(40, 54);
    gfx_sprite_t *niko_down = gfx_MallocSprite(20, 27);
    zx7_Decompress(niko_down, niko_down_compressed);
    gfx_ScaleSprite(niko_down, niko_down_scaled);
    gfx_sprite_t *niko_up_scaled = gfx_MallocSprite(40, 54); 
    gfx_sprite_t *niko_up = gfx_MallocSprite(20, 27);
    zx7_Decompress(niko_up, niko_up_compressed);
    gfx_ScaleSprite(niko_up, niko_up_scaled);
    gfx_sprite_t *niko_left_scaled = gfx_MallocSprite(40, 56); 
    gfx_sprite_t *niko_left = gfx_MallocSprite(20, 28);
    zx7_Decompress(niko_left, niko_left_compressed);
    gfx_ScaleSprite(niko_left, niko_left_scaled);
    gfx_sprite_t *niko_right_scaled = gfx_MallocSprite(40, 56); 
    gfx_sprite_t *niko_right = gfx_MallocSprite(20, 28);
    zx7_Decompress(niko_right, niko_right_compressed);
    gfx_ScaleSprite(niko_right, niko_right_scaled);

    colored_text_t nullColoredText = {0, 0, 0, 0};
    for(int i = 0; i < 4; i++) {
        setColoredText(i, &nullColoredText);
    }

    free(niko_left);
    free(niko_up);
    free(niko_right);
    free(niko_down);

    int curX = 158;
    int curY = 134;

    inventory_init();

    room_start_loadRoom();

    gfx_sprite_t *behind_niko = gfx_MallocSprite(40, 56);
    gfx_GetSprite(behind_niko, curX, curY);

    gfx_sprite_t *curSprite = niko_down_scaled;
    gfx_TransparentSprite(curSprite, curX, curY);

    uint8_t curDir = Down;

    uint8_t moveSpeed = 16;

    sk_key_t key;
    while((key = os_GetCSC()) != sk_Clear && !endProgram) {
        if(renderNiko) gfx_Sprite(behind_niko, curX, curY);
        checkAndWarp(&curX, &curY, curSprite->width, curSprite->height);
        int tryX = curX;
        int tryY = curY;
        if(!inDialogue && renderNiko) {
            switch (key)
            {
            case sk_Right:
                if (inventoryRendering)
                {
                    inventory_moveCursor(INVENTORY_CURSORDIR_RIGHT);
                }
                else
                {
                    tryX += moveSpeed;
                    curDir = Right;
                    curSprite = niko_right_scaled;
                }
                break;

            case sk_Left:
                if (inventoryRendering)
                {
                    inventory_moveCursor(INVENTORY_CURSORDIR_LEFT);
                } else {
                    tryX -= moveSpeed;
                    curDir = Left;
                    curSprite = niko_left_scaled;
                }
                break;

            case sk_Up:
                if (inventoryRendering)
                {
                    inventory_moveCursor(INVENTORY_CURSORDIR_UP);
                } else {
                    tryY -= moveSpeed;
                    curDir = Up;
                    curSprite = niko_up_scaled;
                }
                break;

            case sk_Down:
                if (inventoryRendering)
                {
                    inventory_moveCursor(INVENTORY_CURSORDIR_DOWN);
                } else {
                    tryY += moveSpeed;
                    curDir = Down;
                    curSprite = niko_down_scaled;
                }
                break;

            case sk_Add:
                inventory_toggle();
                break;

            default:
                break;
            }
        }

        if(key == sk_Enter) {
            if(renderNiko && !inventoryRendering) {
                if (!inDialogue)
                {
                    raycastInteractable(curDir, curX, curY, niko_down_scaled->width, niko_down_scaled->height);
                }
                else
                    hideDialogue();
            } else if(inventoryRendering) {
                inventory_selectHighlightedItem();
            }
            /*if(inComputerCutscene) {
                advanceCutscene();
            }*/
        }

        if(tryY != curY || tryX != curX) {
            bool blocked = checkCollision(tryX, tryY, curSprite->width, curSprite->height);
            if (!blocked)
            {
                curX = tryX;
                curY = tryY;
            }
        }

        if(renderNiko) {
            gfx_GetSprite(behind_niko, curX, curY);
            gfx_TransparentSprite(curSprite, curX, curY);
        }
        if(inDialogue) drawDialogue();
        if(inventoryRendering) inventory_renderInventory();
        if(gfxActive) gfx_BlitBuffer();        
    };

    inventory_clean();

    free(niko_down_scaled);
    free(niko_up_scaled);
    free(niko_right_scaled);
    free(niko_left_scaled);

    gfx_End();
}