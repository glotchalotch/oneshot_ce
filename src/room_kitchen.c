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

void fridgeCutscene() {
    static uint8_t fridgeCutsceneState = 0;
    switch(fridgeCutsceneState) {
        case 0: {
            const char* dialogue[3] = {"", "[Inside the refrigerator is a", "bottle of alcohol.]"};
            setOnDialogueHide(fridgeCutscene);
            colored_text_t greenText = {2, 0, 17, COLOR_TRANSPARENT_GREEN};
            setColoredText(0, &greenText);
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
            fridgeCutsceneState++;
            break;
        }
        case 1: {
            const char* dialogue[3] = {"", "[Niko takes it.]", ""};
            showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
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
    unzipScaleDrawSprite(room_house_kitchen_bg1_compressed, room_house_kitchen_bg1_width, room_house_kitchen_bg1_height, 2, 0, 0);
    unzipScaleDrawSprite(room_house_kitchen_bg2_compressed, room_house_kitchen_bg2_width, room_house_kitchen_bg2_height, 2, 80, 0);
    unzipScaleDrawSprite(room_house_kitchen_bg3_compressed, room_house_kitchen_bg3_width, room_house_kitchen_bg3_height, 2, 160, 0);
    unzipScaleDrawSprite(room_house_kitchen_bg4_compressed, room_house_kitchen_bg4_width, room_house_kitchen_bg4_height, 2, 216, 0);
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(262, 0, GFX_LCD_WIDTH - 262, GFX_LCD_HEIGHT); // kitchen doesnt take up full width so filling the rest of the screen w/ black
}

void room_kitchen_loadRoom() {
    room_kitchen_renderRoom();

    bounding_box_t boxes[BBOX_ARR_SIZE];
    makeEmptyBoundingBoxArray(boxes);

    bounding_box_t leftBox = {0, 28, 19, 212};
    boxes[0] = leftBox;

    bounding_box_t bottomBox = {19, 238, 241, 2};
    boxes[1] = bottomBox;

    bounding_box_t rightBox = {259, 24, 36, 216};
    boxes[2] = rightBox;

    bounding_box_t topBox = {110, 0, 149, 116};
    boxes[3] = topBox;

    bounding_box_t tableBox = {154, 200, 103, 37};
    boxes[4] = tableBox;

    bounding_box_t sinkBox = {18, 156, 91, 52};
    boxes[5] = sinkBox;

    bounding_box_t fridgeBox = {215, 74, 39, 75};
    boxes[6] = fridgeBox;

    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    makeEmptyInteractableArray(interactables);

    interactable_t fridgeInteractable = {fridgeBox, &fridgeCutscene, NULL};
    interactables[0] = fridgeInteractable;

    warp_t warps[WARP_ARR_SIZE];
    makeEmptyWarpArray(warps);

    warp_t livingroomWarp = {{19, 28, 88, 60}, &room_livingroom_loadRoom, 199, 174};
    warps[0] = livingroomWarp;

    setBoundingBoxes(boxes);
    setInteractables(interactables);
    setWarps(warps);
}