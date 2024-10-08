#include <tice.h>
#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "dialogue.h"
#include "collision.h"
#include "room_start.h"
#include "inventory.h"
#include "color.h"
#include "input.h"

#define DIR_DOWN 0
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

enum direction
{
    Down,
    Up,
    Left,
    Right
};
bool renderNiko = true;
bool gfxActive = true;
bool endProgram = false;

int curX = 158;
int curY = 134;
int tryX;
int tryY;
uint8_t curDir = DIR_DOWN;
uint8_t moveSpeed = 16;

gfx_sprite_t *behind_niko;
gfx_sprite_t *curSprite;

gfx_sprite_t *niko_down_scaled;
gfx_sprite_t *niko_up_scaled;
gfx_sprite_t *niko_left_scaled;
gfx_sprite_t *niko_right_scaled;

void initGfx()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(COLOR_TRANSPARENT_GREEN);
    gfx_SetTextScale(1, 2);
    gfx_SetTextFGColor(COLOR_WHITE);
}

void unzipScaleDrawSprite(void *compressed_sprite, uint8_t compressedW, uint8_t compressedH, uint8_t scaleFactor, int x, int y)
{
    gfx_sprite_t *sprite = gfx_MallocSprite(compressedW * scaleFactor, compressedH * scaleFactor);
    gfx_sprite_t *spriteUncompressed = gfx_MallocSprite(compressedW, compressedH);
    zx7_Decompress(spriteUncompressed, compressed_sprite);
    gfx_ScaleSprite(spriteUncompressed, sprite);
    gfx_TransparentSprite(sprite, x, y);
    free(sprite);
    free(spriteUncompressed);
}

void setRenderNiko(bool render)
{
    renderNiko = render;
}

void setGfxActive(bool active)
{
    gfxActive = active;
}

void markEndProgram()
{
    endProgram = true;
}

void defaultInputHandler(sk_key_t key)
{
    tryX = curX;
    tryY = curY;
    switch (key)
    {
    case sk_Up:
        tryY -= moveSpeed;
        curDir = DIR_UP;
        curSprite = niko_up_scaled;
        break;
    case sk_Down:
        tryY += moveSpeed;
        curDir = DIR_DOWN;
        curSprite = niko_down_scaled;
        break;
    case sk_Left:
        tryX -= moveSpeed;
        curDir = DIR_LEFT;
        curSprite = niko_left_scaled;
        break;
    case sk_Right:
        tryX += moveSpeed;
        curDir = DIR_RIGHT;
        curSprite = niko_right_scaled;
        break;
    case sk_Enter:
        raycastInteractable(curDir, curX, curY, niko_down_scaled->width, niko_down_scaled->height);
        break;
    case sk_Add:
        inventory_toggle();
        break;
    case sk_9: {
        bounding_box_t* bboxes = getBoundingBoxes();
        gfx_SetColor(COLOR_LIGHT_BLUE);
        for(uint8_t i = 0; i < BBOX_ARR_SIZE; i++) {
            gfx_Rectangle(bboxes[i].x, bboxes[i].y, bboxes[i].width, bboxes[i].height);
        }

        warp_t* warps = getWarps();
        gfx_SetColor(COLOR_TRANSPARENT_GREEN);
        for(uint8_t i = 0; i < WARP_ARR_SIZE; i++) {
            gfx_Rectangle(warps[i].boundingBox.x, warps[i].boundingBox.y, warps[i].boundingBox.width, warps[i].boundingBox.height);
        }
        break;
    }
        
    }
}

int main()
{
    initGfx();
    // dbg_SetBreakpoint(&inventory_toggle);

    niko_down_scaled = gfx_MallocSprite(40, 54);
    gfx_sprite_t *niko_down = gfx_MallocSprite(20, 27);
    zx7_Decompress(niko_down, niko_down_compressed);
    gfx_ScaleSprite(niko_down, niko_down_scaled);
    niko_up_scaled = gfx_MallocSprite(40, 54);
    gfx_sprite_t *niko_up = gfx_MallocSprite(20, 27);
    zx7_Decompress(niko_up, niko_up_compressed);
    gfx_ScaleSprite(niko_up, niko_up_scaled);
    niko_left_scaled = gfx_MallocSprite(40, 56);
    gfx_sprite_t *niko_left = gfx_MallocSprite(20, 28);
    zx7_Decompress(niko_left, niko_left_compressed);
    gfx_ScaleSprite(niko_left, niko_left_scaled);
    niko_right_scaled = gfx_MallocSprite(40, 56);
    gfx_sprite_t *niko_right = gfx_MallocSprite(20, 28);
    zx7_Decompress(niko_right, niko_right_compressed);
    gfx_ScaleSprite(niko_right, niko_right_scaled);

    colored_text_t nullColoredText = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        setColoredText(i, &nullColoredText);
    }

    free(niko_left);
    free(niko_up);
    free(niko_right);
    free(niko_down);

    inventory_init();

    room_start_loadRoom();

    behind_niko = gfx_MallocSprite(40, 56);
    gfx_GetSprite(behind_niko, curX, curY);

    curSprite = niko_down_scaled;
    gfx_TransparentSprite(curSprite, curX, curY);

    setInputHandler(defaultInputHandler);
    sk_key_t key;
    while ((key = os_GetCSC()) != sk_Clear && !endProgram)
    {
        if (renderNiko)
            gfx_Sprite(behind_niko, curX, curY);
        checkAndWarp(&curX, &curY, curSprite->width, curSprite->height);
        handleInput(key);

        if (tryY != curY || tryX != curX)
        {
            bool blocked = checkCollision(tryX, tryY, curSprite->width, curSprite->height);
            if (!blocked)
            {
                curX = tryX;
                curY = tryY;
            }
        }

        if (renderNiko)
        {
            gfx_GetSprite(behind_niko, curX, curY);
            gfx_TransparentSprite(curSprite, curX, curY);
        }
        if (inDialogue)
            drawDialogue();
        if (inventory_isInventoryRendering())
            inventory_renderInventory();
        if (gfxActive)
            gfx_BlitBuffer();
    };

    free(niko_down_scaled);
    free(niko_up_scaled);
    free(niko_right_scaled);
    free(niko_left_scaled);

    gfx_End();
}