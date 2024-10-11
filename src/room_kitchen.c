#include <tice.h>
#include <graphx.h>
#include "gfx/gfx.h"
#include "main.h"
#include "color.h"
#include "collision.h"
#include "dialogue.h"
#include "inventory.h"
#include "room_kitchen.h"
#include "room_livingroom.h"
#include "tile.h"

void fridgeCutscene() {
    static uint8_t fridgeCutsceneState = 0;
    switch(fridgeCutsceneState) {
        case 0: {
            const char* dialogue[3] = {"", "[Inside the refrigerator is a", "bottle of alcohol.]"};
            setOnDialogueHide(fridgeCutscene);
            colored_text_t greenText = {2, 0, 17, COLOR_TRANSPARENT_GREEN};
            setColoredText(0, &greenText);
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL, false);
            fridgeCutsceneState++;
            break;
        }
        case 1: {
            const char* dialogue[3] = {"", "[Niko takes it.]", ""};
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL, true);
            fridgeCutsceneState++;
            break;
        }
        case 2: {
            static item_t bottleItem = {ITEM_HOUSE_ALCOHOL, "bottle of alcohol"};
            inventory_addItem(&bottleItem);
            fridgeCutsceneState++;
            break;
        }
    }
}

void room_kitchen_renderRoom() {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    uint8_t bgMap[63] = {
        126, 126, 127, 98, 58, 58, 58, 58, 58,
        58, 107, 108, 107, 93, 93, 93, 93, 93,
        58, 108, 107, 108, 93, 93, 93, 93, 93,
        58, 107, 108, 107, 108, 107, 108, 107, 108,
        58, 108, 107, 108, 107, 108, 107, 108, 107,
        58, 107, 108, 107, 108, 107, 108, 107, 108,
        58, 108, 107, 108, 107, 108, 107, 108, 107
        };

    drawBgTilemap(bgMap, 7, 9, 0, 0);

    uint8_t fgMap[48] = {
        83, 83, 83, 83, 83, 83, 48, 49,
        83, 83, 83, 83, 83, 83, 56, 57,
        83, 83, 83, 83, 83, 83, 64, 65,
        43, 44, 45, 83, 83, 83, 83, 83,
        51, 52, 53, 83, 83, 59, 60, 83,
        83, 83, 83, 83, 75, 67, 68, 76
    };

    drawFgTilemap(fgMap, 6, 8, 16, 16);

    expandQuadrant(3);
    expandQuadrant(2);
    expandQuadrant(1);
    expandQuadrant(0);
}

void room_kitchen_loadRoom() {
    room_kitchen_renderRoom();

    bounding_box_t boxes[BBOX_ARR_SIZE];
    makeEmptyBoundingBoxArray(boxes);

    bounding_box_t leftBox = {0, 28, 32, 212};
    boxes[0] = leftBox;

    bounding_box_t bottomBox = {32, 223, 256, 17};
    boxes[1] = bottomBox;

    bounding_box_t rightBox = {288, 32, 32, 208};
    boxes[2] = rightBox;

    bounding_box_t topBox = {128, 0, 160, 98};
    boxes[3] = topBox;

    bounding_box_t tableBox = {176, 184, 95, 40};
    boxes[4] = tableBox;

    bounding_box_t sinkBox = {32, 144, 96, 46};
    boxes[5] = sinkBox;

    bounding_box_t fridgeBox = {240, 50, 39, 75};
    boxes[6] = fridgeBox;

    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    makeEmptyInteractableArray(interactables);

    interactable_t fridgeInteractable = {fridgeBox, &fridgeCutscene, NULL};
    interactables[0] = fridgeInteractable;

    warp_t warps[WARP_ARR_SIZE];
    makeEmptyWarpArray(warps);

    warp_t livingroomWarp = {{32, 32, 96, 32}, &room_livingroom_loadRoom, 199, 174};
    warps[0] = livingroomWarp;

    setBoundingBoxes(boxes);
    setInteractables(interactables);
    setWarps(warps);
}