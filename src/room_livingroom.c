#include <tice.h>
#include <graphx.h>
#include "gfx/gfx.h"
#include "collision.h"
#include "main.h"
#include "dialogue.h"
#include "color.h"
#include "inventory.h"
#include "room_start.h"
#include "room_kitchen.h"
#include "tile.h"

void tvCutscene()
{
    item_t *currentItem = inventory_getCurrentItem();
    if (currentItem->id == ITEM_HOUSE_WETBRANCH)
    {
        static uint8_t tvCutsceneState = 0;
        switch (tvCutsceneState)
        {
        case 0:
        {
            const char *dialogue[3] = {"", "[Niko readies the branch...]", ""};
            setOnDialogueHide(&tvCutscene);
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
            tvCutsceneState++;
            break;
        }
        case 1:
        {
            const char *dialogue[3] = {"Ah!", "", ""};
            showDialogue(dialogue, DIALOGUE_TYPE_PERSONAL);
            tvCutsceneState++;
            break;
        }
        case 2:
        {
            const char *dialogue[3] = {"", "[The branch lights with a blue flame.]", ""};
            setOnDialogueHide(NULL);
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
            inventory_removeItem(inventory_getItemIndex(currentItem));
            static item_t litBranch = {ITEM_HOUSE_LITBRANCH, "lit branch"};
            uint8_t litBranchIndex = inventory_addItem(&litBranch);
            inventory_selectItem(litBranchIndex);
            tvCutsceneState++;
            break;
        }
        }
    }
    else
    {
        const char *dialogue[3] = {"", "[The TV gives off a dangerous looking spark.]", ""};
        colored_text_t yellowText = {1, 38, 5, COLOR_YELLOW};
        setColoredText(0, &yellowText);
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
    }
}

void keyCutscene() {
    static uint8_t keyCutsceneState = 0;
    static gfx_sprite_t* behindGlint;
    switch(keyCutsceneState) {
        case 0: {
            behindGlint = gfx_MallocSprite(key_glint_width, key_glint_height);
            gfx_GetSprite(behindGlint, 178, 147);

            unzipScaleDrawSprite(fire_compressed, fire_width, fire_height, 1, 230, 99);
            unzipScaleDrawSprite(key_glint_compressed, key_glint_width, key_glint_height, 1, 178, 147);

            bounding_box_t keyBox = {178, 147, key_glint_width, key_glint_height};
            interactable_t keyInteractable = {keyBox, &keyCutscene, NULL};

            interactable_t* interactables = getInteractables();
            interactables[2] = keyInteractable;
            setInteractables(interactables);

            keyCutsceneState = 1;
            break;
        }
        case 1: {
            static item_t keyItem = {ITEM_HOUSE_BASEMENTKEY, "basement key"};
            inventory_addItem(&keyItem);

            gfx_Sprite(behindGlint, 178, 147);
            keyCutsceneState++;
            break;
        }
    }
}

void fireplaceCutscene()
{
    static uint8_t fireplaceCutsceneState = 0;
    if (inventory_getCurrentItem()->id == ITEM_HOUSE_LITBRANCH)
    {
        inventory_removeItem(inventory_getItemIndex(inventory_getCurrentItem()));

        fireplaceCutsceneState = 2;
        keyCutscene();
    }
    else
    {
        switch (fireplaceCutsceneState)
        {
            case 0:
            {
                const char *dialogue[3] = {"", "[The wood in the fireplace looks like", "it was never used.]"};
                setOnDialogueHide(&fireplaceCutscene);
                fireplaceCutsceneState++;
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                break;
            }
            case 1:
            {
                const char *dialogue[3] = {"", "[Perhaps it could be lit.]", ""};
                fireplaceCutsceneState = 0;
                colored_text_t blueText = {1, 21, 3, COLOR_LIGHT_BLUE};
                setColoredText(0, &blueText);
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                setOnDialogueHide(NULL);
                break;
            }
        }
    }
}

void room_livingroom_renderRoom()
{
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    uint8_t bgMap[70] = {
        58, 85, 1, 85, 85, 85, 85, 85, 85, 58,
        3, 93, 2, 93, 93, 93, 93, 93, 93, 4,
        6, 97, 98, 99, 97, 98, 99, 97, 98, 6,
        9, 81, 82, 109, 110, 110, 110, 111, 82, 10,
        111, 89, 90, 117, 118, 118, 118, 119, 90, 104,
        127, 97, 98, 125, 126, 126, 126, 127, 98, 58,
        58, 58, 58, 58, 58, 58, 107, 108, 107, 58 
    };

    drawBgTilemap(bgMap, 7, 10, 0, 16);

    uint8_t fgMap[40] = {
        83, 83, 83, 83, 83, 9, 10, 11,
        83, 83, 83, 33, 83, 12, 13, 14,
        83, 83, 40, 41, 42, 0, 1, 2,
        83, 83, 83, 83, 83, 83, 83, 83,
        81, 82, 83, 83, 83, 83, 83, 83
    };

    drawFgTilemap(fgMap, 5, 8, 16, 16);

    uint8_t outlineMap[80] = {
        4, 7, 7, 7, 7, 7, 7, 7, 7, 4,
        8, 4, 4, 4, 4, 4, 4, 4, 4, 6,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
        1, 1, 1, 1, 1, 2, 4, 4, 4, 3
    };

    drawOutlineTilemap(outlineMap, 8, 10, 0, 0);

    gfx_SetColor(COLOR_OUTLINE);
    gfx_SetPixel(15, 15);
    gfx_SetPixel(144, 15);

    expandQuadrant(3);
    expandQuadrant(2);
    expandQuadrant(1);
    expandQuadrant(0);

    /*unzipScaleDrawSprite(room_house_livingroom_bg1_compressed, room_house_livingroom_bg1_width, room_house_livingroom_bg1_height, 2, 0, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg2_compressed, room_house_livingroom_bg2_width, room_house_livingroom_bg2_height, 2, 80, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg3_compressed, room_house_livingroom_bg3_width, room_house_livingroom_bg3_height, 2, 160, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg4_compressed, room_house_livingroom_bg4_width, room_house_livingroom_bg4_height, 2, 240, 0);*/
}

void room_livingroom_loadRoom()
{
    room_livingroom_renderRoom();

    bounding_box_t boundingBoxes[BBOX_ARR_SIZE];
    makeEmptyBoundingBoxArray(boundingBoxes);

    bounding_box_t backWallBox = {33, 5, 173, 90};
    boundingBoxes[0] = backWallBox;

    bounding_box_t leftWallBox = {0, 68, 32, 93};
    boundingBoxes[1] = leftWallBox;

    bounding_box_t bottomWallBox = {0, 220, 138, 20};
    boundingBoxes[2] = bottomWallBox;

    bounding_box_t bottomRightCornerBox = {273, 185, 47, 55};
    boundingBoxes[3] = bottomRightCornerBox;

    bounding_box_t rightWallBox = {285, 63, 35, 91};
    boundingBoxes[4] = rightWallBox;

    bounding_box_t tvBox = {109, 97, 60, 28};
    boundingBoxes[5] = tvBox;

    bounding_box_t fireplaceBox = {202, 95, 82, 30};
    boundingBoxes[6] = fireplaceBox;

    bounding_box_t holeBox = {33, 158, 51, 24};
    boundingBoxes[7] = holeBox;

    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    makeEmptyInteractableArray(interactables);

    interactable_t tvInteractable = {tvBox, &tvCutscene, NULL};
    interactables[0] = tvInteractable;

    interactable_t fireplaceInteractable = {fireplaceBox, &fireplaceCutscene, NULL};
    interactables[1] = fireplaceInteractable;

    warp_t warps[WARP_ARR_SIZE];
    makeEmptyWarpArray(warps);

    warp_t bedroomWarp = {{61, 95, 30, 15}, &room_start_loadRoom, 222, 150};
    warps[0] = bedroomWarp;

    warp_t kitchenWarp = {{183, 220, 90, 20}, &room_kitchen_loadRoom, 38, 76};
    warps[1] = kitchenWarp;

    setBoundingBoxes(boundingBoxes);
    setInteractables(interactables);
    setWarps(warps);
}