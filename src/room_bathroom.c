#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "color.h"
#include "collision.h"
#include "dialogue.h"
#include "inventory.h"
#include "room_start.h"

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

    gfx_sprite_t* bg1 = gfx_MallocSprite(room_house_bathroom_bg1_width * 2, room_house_bathroom_bg1_height * 2);
    gfx_sprite_t* bg1_uncompressed = gfx_MallocSprite(room_house_bathroom_bg1_width, room_house_bathroom_bg1_height);
    zx7_Decompress(bg1_uncompressed, room_house_bathroom_bg1_compressed);
    gfx_ScaleSprite(bg1_uncompressed, bg1);
    free(bg1_uncompressed);
    gfx_Sprite(bg1, 40, 0);
    free(bg1);

    gfx_sprite_t* bg2 = gfx_MallocSprite(room_house_bathroom_bg2_width * 2, room_house_bathroom_bg2_height * 2);
    gfx_sprite_t* bg2_uncompressed = gfx_MallocSprite(room_house_bathroom_bg2_width, room_house_bathroom_bg2_height);
    zx7_Decompress(bg2_uncompressed, room_house_bathroom_bg2_compressed);
    gfx_ScaleSprite(bg2_uncompressed, bg2);
    free(bg2_uncompressed);
    gfx_Sprite(bg2, (room_house_bathroom_bg1_width * 2) + 40, 0);
    free(bg2);

    gfx_sprite_t* bg3 = gfx_MallocSprite(room_house_bathroom_bg3_width * 2, room_house_bathroom_bg3_height * 2);
    gfx_sprite_t* bg3_uncompressed = gfx_MallocSprite(room_house_bathroom_bg3_width, room_house_bathroom_bg3_height);
    zx7_Decompress(bg3_uncompressed, room_house_bathroom_bg3_compressed);
    gfx_ScaleSprite(bg3_uncompressed, bg3);
    free(bg3_uncompressed);
    gfx_Sprite(bg3, (room_house_bathroom_bg1_width * 2) + (room_house_bathroom_bg2_width * 2) + 40, 0);
    free(bg3);
}

void room_bathroom_loadRoom() {
    bounding_box_t boxes[BBOX_ARR_SIZE];
    makeEmptyBoundingBoxArray(boxes);

    bounding_box_t backWallBox = {40, 0, 200, 120};
    boxes[0] = backWallBox;

    bounding_box_t backWallBox2 = {242, 78, 42, 120};
    boxes[1] = backWallBox2;

    bounding_box_t toiletBox = {48, 90, 28, 66};
    boxes[2] = toiletBox;

    bounding_box_t bottomBox = {0, 240, 320, 16};
    boxes[3] = bottomBox;

    bounding_box_t rightBox = {284, 0, 1, 240};
    boxes[4] = rightBox;

    bounding_box_t bathtubBox = {42, 196, 82, 42};
    boxes[5] = bathtubBox;

    bounding_box_t leftBox = {0, 0, 40, 320};
    boxes[6] = leftBox;

    bounding_box_t plantBox = {208, 120, 34, 34};
    boxes[7] = plantBox;

    bounding_box_t sinkBox = {122, 118, 44, 26};
    boxes[8] = sinkBox;


    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    makeEmptyInteractableArray(interactables);

    interactable_t plantInteractable = {plantBox, &plantCutscene, NULL};
    interactables[0] = plantInteractable;

    warp_t warps[WARP_ARR_SIZE];
    makeEmptyWarpArray(warps);

    warp_t warp = {{260, 198, 22, 40}, &room_start_loadRoom, 14, 136};
    warps[0] = warp;

    setBoundingBoxes(boxes);
    setInteractables(interactables);
    setWarps(warps);

    room_bathroom_renderRoom();
}