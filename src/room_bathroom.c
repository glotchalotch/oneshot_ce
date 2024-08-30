#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "color.h"
#include "collision.h"
#include "dialogue.h"
#include "inventory.h"
#include "room_start.h"
#include "main.h"
#include "tile.h"

void plantCutscene() {
    static uint8_t plantCutsceneState = 0;
    switch(plantCutsceneState) {
        case 0: {
            colored_text_t textStruct = {1, 3, 14, COLOR_GRAY};
            setColoredText(0, &textStruct);
            const char *dialogue[3] = {"", "[A withered plant sits here, stiff and dry.]", ""};
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
            setOnDialogueHide(&plantCutscene);
            plantCutsceneState++;
            break;
        }
        case 1: {
            const char *dialogue[3] = {"", "[A branch snaps off.]", ""};
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
            plantCutsceneState++;
            break;
        }
        case 2: {
            static item_t branch = {ITEM_HOUSE_BRANCH, "dry branch"};
            inventory_addItem(&branch);
            plantCutsceneState++;
            break;
        }
    }
}

void room_bathroom_renderRoom() {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    uint8_t bgMap[36] = {3, 4, 3, 4, 3, 58,
                        85, 85, 85, 85, 85, 58,
                        85, 85, 85, 85, 85, 3,
                        81, 82, 83, 81, 82, 25,
                        97, 98, 109, 111, 98, 85,
                        89, 90, 125, 127, 90, 96};

    drawBgTilemap(bgMap, 6, 6, 34, 12);

    uint8_t fgMap[20] = {50, 83, 39, 83, 83,
                        58, 83, 47, 83, 80,
                        83, 83, 83, 83, 83,
                        73, 74, 83, 83, 83};
    
    drawFgTilemap(fgMap, 4, 5, 34, 44);

    uint8_t outlineMap[48] = {5, 1, 1, 1, 1, 1, 3, 4,
                            5, 4, 4, 4, 4, 4, 6, 4,
                            5, 4, 4, 4, 4, 4, 4, 3,
                            5, 4, 4, 4, 4, 4, 4, 3,
                            5, 4, 4, 4, 4, 4, 4, 3,
                            5, 7, 7, 7, 7, 7, 7, 3};

    drawOutlineTilemap(outlineMap, 6, 8, 18, 12);

    gfx_SetColor(COLOR_OUTLINE);
    gfx_SetPixel(130, 43);

    expandQuadrant(3);
    expandQuadrant(2);
    expandQuadrant(1);
    expandQuadrant(0);

    /*unzipScaleDrawSprite(room_house_bathroom_bg1_compressed, room_house_bathroom_bg1_width, room_house_start_bg1_height, 2, 40, 0);
    unzipScaleDrawSprite(room_house_bathroom_bg2_compressed, room_house_bathroom_bg2_width, room_house_start_bg2_height, 2, 120, 0);
    unzipScaleDrawSprite(room_house_bathroom_bg3_compressed, room_house_bathroom_bg3_width, room_house_bathroom_bg3_height, 2, 200, 0);*/
}

void room_bathroom_loadRoom() {
    bounding_box_t boxes[BBOX_ARR_SIZE];
    makeEmptyBoundingBoxArray(boxes);

    bounding_box_t backWallBox = {68, 26, 159, 93};
    boxes[0] = backWallBox;

    bounding_box_t backWallBox2 = {228, 88, 32, 97};
    boxes[1] = backWallBox2;

    bounding_box_t toiletBox = {68, 120, 32, 32};
    boxes[2] = toiletBox;

    bounding_box_t bottomBox = {68, 214, 192, 23};
    boxes[3] = bottomBox;

    bounding_box_t rightBox = {260, 88, 8, 128};
    boxes[4] = rightBox;

    bounding_box_t bathtubBox = {68, 182, 64, 32};
    boxes[5] = bathtubBox;

    bounding_box_t leftBox = {60, 24, 8, 192};
    boxes[6] = leftBox;

    bounding_box_t plantBox = {195, 120, 32, 32};
    boxes[7] = plantBox;

    bounding_box_t sinkBox = {132, 119, 32, 21};
    boxes[8] = sinkBox;


    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    makeEmptyInteractableArray(interactables);

    interactable_t plantInteractable = {plantBox, &plantCutscene, NULL};
    interactables[0] = plantInteractable;

    warp_t warps[WARP_ARR_SIZE];
    makeEmptyWarpArray(warps);

    warp_t warp = {{228, 184, 32, 32}, &room_start_loadRoom, 14, 136};
    warps[0] = warp;

    setBoundingBoxes(boxes);
    setInteractables(interactables);
    setWarps(warps);

    room_bathroom_renderRoom();
}