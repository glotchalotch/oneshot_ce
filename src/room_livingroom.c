#include <tice.h>
#include "gfx/gfx.h"
#include "collision.h"
#include "main.h"
#include "dialogue.h"
#include "color.h"
#include "inventory.h"
#include "room_start.h"
#include "room_kitchen.h"

void tvCutscene() {
    item_t* currentItem = inventory_getCurrentItem();
    if(currentItem->id == ITEM_HOUSE_WETBRANCH) {
        static uint8_t tvCutsceneState = 0;
        switch(tvCutsceneState) {
            case 0: {
                const char* dialogue[3] = {"", "[Niko readies the branch...]", ""};
                setOnDialogueHide(&tvCutscene);
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                tvCutsceneState++;
                break;
            }
            case 1: {
                const char* dialogue[3] = {"Ah!", "", ""};
                showDialogue(dialogue, DIALOGUE_TYPE_PERSONAL);
                tvCutsceneState++;
                break;
            }
            case 2: {
                const char* dialogue[3] = {"", "[The branch lights with a blue flame.]", ""};
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
    } else {
        const char* dialogue[3] = {"", "[The TV gives off a dangerous looking spark.]", ""};
        colored_text_t yellowText = {1, 38, 5, COLOR_YELLOW};
        setColoredText(0, &yellowText);
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
    }
}

void fireplaceCutscene() {
    static uint8_t fireplaceCutsceneState = 0;
    if(inventory_getCurrentItem()->id == ITEM_HOUSE_LITBRANCH) {
        // todo
    } else {
        switch(fireplaceCutsceneState) {
            case 0: {
                const char* dialogue[3] = {"", "[The wood in the fireplace looks like", "it was never used.]"};
                setOnDialogueHide(&fireplaceCutscene);
                fireplaceCutsceneState++;
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                break;
            }
            case 1: {
                const char* dialogue[3] = {"", "[Perhaps it could be lit.]", ""};
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

void room_livingroom_renderRoom() {
    unzipScaleDrawSprite(room_house_livingroom_bg1_compressed, room_house_livingroom_bg1_width, room_house_livingroom_bg1_height, 2, 0, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg2_compressed, room_house_livingroom_bg2_width, room_house_livingroom_bg2_height, 2, 80, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg3_compressed, room_house_livingroom_bg3_width, room_house_livingroom_bg3_height, 2, 160, 0);
    unzipScaleDrawSprite(room_house_livingroom_bg4_compressed, room_house_livingroom_bg4_width, room_house_livingroom_bg4_height, 2, 240, 0);
}

void room_livingroom_loadRoom() {
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