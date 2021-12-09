#include <tice.h>
#include <graphx.h>

#include "gfx/gfx.h"
#include "dialogue.h"
#include "collision.h"
#include "room_test.h"

enum direction {Down, Up, Left, Right};
bool renderNiko = true;
bool gfxActive = true;
bool endProgram = false;

void initGfx() {
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetTextScale(1, 2);
    gfx_SetTextFGColor(2);
}

int main() {
    initGfx();    

    gfx_sprite_t *niko_down_scaled = gfx_MallocSprite(40, 54); 
    gfx_ScaleSprite(niko_down, niko_down_scaled);
    gfx_sprite_t *niko_up_scaled = gfx_MallocSprite(40, 54); 
    gfx_ScaleSprite(niko_up, niko_up_scaled);
    gfx_sprite_t *niko_left_scaled = gfx_MallocSprite(40, 56); 
    gfx_ScaleSprite(niko_left, niko_left_scaled);
    gfx_sprite_t *niko_right_scaled = gfx_MallocSprite(40, 56); 
    gfx_ScaleSprite(niko_right, niko_right_scaled);
    gfx_sprite_t *behind_niko = gfx_MallocSprite(40, 56);
    gfx_GetSprite(behind_niko, 0, 0);

    gfx_sprite_t *curSprite = niko_down_scaled;
    gfx_TransparentSprite(curSprite, 160, 144);

    loadRoom();

    int curX = 128;
    int curY = 144;
    uint8_t curDir = Down;

    uint8_t moveSpeed = 16;

    sk_key_t key;
    while((key = os_GetCSC()) != sk_Clear && !endProgram) {
        if(renderNiko) gfx_Sprite(behind_niko, curX, curY);
        int tryX = curX;
        int tryY = curY;
        if(!inDialogue && renderNiko) {
            switch (key)
            {
            case sk_Right:
                tryX += moveSpeed;
                curDir = Right;
                curSprite = niko_right_scaled;
                break;

            case sk_Left:
                tryX -= moveSpeed;
                curDir = Left;
                curSprite = niko_left_scaled;
                break;

            case sk_Up:
                tryY -= moveSpeed;
                curDir = Up;
                curSprite = niko_up_scaled;
                break;

            case sk_Down:
                tryY += moveSpeed;
                curDir = Down;
                curSprite = niko_down_scaled;
                break;

            default:
                break;
            }
        }

        if(key == sk_Enter) {
            if(renderNiko) {
                if (!inDialogue)
                {
                    raycastInteractable(curDir, curX, curY, niko_down_scaled->width, niko_down_scaled->height);
                }
                else
                    hideDialogue();
            }
            if(inComputerCutscene) {
                advanceCutscene();
            }
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
        if(gfxActive) gfx_BlitBuffer();        
    };

    free(niko_down_scaled);
    free(niko_up_scaled);
    free(niko_right_scaled);
    free(niko_left_scaled);

    gfx_End();
}