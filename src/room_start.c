#include <graphx.h>
#include <tice.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "dialogue.h"
#include "main.h"
#include "collision.h"
#include "room_start.h"
#include "color.h"
#include "inventory.h"
#include "room_bathroom.h"

uint8_t computerCutsceneState = 0;
bool inComputerCutscene = false;
uint8_t remoteCutsceneState = 0;

gfx_sprite_t* behind_remote;

void beginCutscene() {
    /*gfx_FillScreen(COLOR_LIGHT_BLUE);
    computerCutsceneState = 1;
    renderNiko = false;
    inComputerCutscene = true;*/
}

void getRemote() {
    static item_t remote = {ITEM_HOUSE_REMOTE, "remote"};
    inventory_addItem(&remote);
    gfx_Sprite(behind_remote, 262, 150);
    free(behind_remote);
    removeInteractable(1);
}

void remoteCutscene() {
    if(remoteCutsceneState == 0) {
        const char *dialogue[3] = {"", "[There's a TV remote here.]", ""};
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
        setOnDialogueHide(&remoteCutscene);
        remoteCutsceneState = 1;
    } else {
        const char *dialogue[3] = {"", "[Niko picks it up.]", ""};
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
        setOnDialogueHide(&getRemote);
    }
}

void computerCutscene() {
    /*const char* offDialogue[3] = {"It's a computer!", "It's turned off.", ""};
    const char* onDialogue[3] = {"It turned on!", "", ""};
    if(computerCutsceneState == 0) {
        showDialogue(offDialogue, DIALOGUE_TYPE_PERSONAL);
    } else if(computerCutsceneState == 1) {
        showDialogue(onDialogue, DIALOGUE_TYPE_PERSONAL);
        gfx_sprite_t* computer_on = gfx_MallocSprite(computer_on_width, computer_on_height);
        zx7_Decompress(computer_on, computer_on_compressed);
        gfx_TransparentSprite(computer_on, 118, 48);
        setOnDialogueHide(beginCutscene);
    }*/
}

void room_start_renderRoom() {
    //rendering the bg all at once puts it on the verge of running out of ram
    //so i have to juggle mem a bit like this
    //probably for the best anyway
    gfx_sprite_t* room_house_start_bg1 = gfx_MallocSprite(room_house_start_bg1_width * 2, room_house_start_bg1_height * 2);
    gfx_sprite_t* room_house_start_bg1_uncompressed = gfx_MallocSprite(room_house_start_bg1_width, room_house_start_bg1_height);
    zx7_Decompress(room_house_start_bg1_uncompressed, room_house_start_bg1_compressed);
    gfx_ScaleSprite(room_house_start_bg1_uncompressed, room_house_start_bg1);
    gfx_Sprite(room_house_start_bg1, 0, 0);
    free(room_house_start_bg1_uncompressed);
    free(room_house_start_bg1);
    
    gfx_sprite_t* room_house_start_bg2 = gfx_MallocSprite(room_house_start_bg2_width * 2, room_house_start_bg2_height * 2);
    gfx_sprite_t* room_house_start_bg2_uncompressed = gfx_MallocSprite(room_house_start_bg2_width, room_house_start_bg2_height);
    zx7_Decompress(room_house_start_bg2_uncompressed, room_house_start_bg2_compressed);
    gfx_ScaleSprite(room_house_start_bg2_uncompressed, room_house_start_bg2);
    gfx_Sprite(room_house_start_bg2, 160, 0);
    free(room_house_start_bg2_uncompressed);
    free(room_house_start_bg2);

    gfx_sprite_t* remote = gfx_MallocSprite(remote_width, remote_height);
    behind_remote = gfx_MallocSprite(remote_width, remote_height);
    gfx_GetSprite(behind_remote, 262, 150);
    zx7_Decompress(remote, remote_compressed);
    gfx_TransparentSprite(remote, 262, 150);
    free(remote);
}

void room_start_loadRoom() {
    room_start_renderRoom();

    bounding_box_t boundingBoxes[BBOX_ARR_SIZE];
    for(int i = 0; i < BBOX_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        boundingBoxes[i] = bbox;
    }
    bounding_box_t backWallBox = {66, 0, 254, 99};
    boundingBoxes[0] = backWallBox;

    bounding_box_t objBox = {129, 98, 121, 33};
    boundingBoxes[1] = objBox;

    bounding_box_t frontWallBox = {0, 66, 66, 97};
    boundingBoxes[2] = frontWallBox;

    bounding_box_t bottomBox2 = {0, 196, 222, 44};
    boundingBoxes[3] = bottomBox2;

    bounding_box_t bottomBox3 = {224, 228, 96, 12};
    boundingBoxes[4] = bottomBox3;

    bounding_box_t bottomBox4 = {262, 196, 58, 34};
    boundingBoxes[5] = bottomBox4;

    bounding_box_t rightWallBox = {317, 0, 3, 198};
    boundingBoxes[6] = rightWallBox;

    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    for(int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        interactable_t inter = {bbox, NULL, NULL};
        interactables[i] = inter;
    }
    bounding_box_t computerBox = {129, 86, 58, 46};
    interactables[0].boundingBox = computerBox;
    interactables[0].onHit = (void(*)(void*))&computerCutscene;

    if(remoteCutsceneState < 1) {
        bounding_box_t remoteBox = {262, 150, 23, 26};
        interactables[1].boundingBox = remoteBox;
        interactables[1].onHit = (void(*)(void*))&remoteCutscene;
    }

    warp_t warps[WARP_ARR_SIZE];
    for(int i = 0; i < WARP_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        warp_t inter = {bbox, NULL, 0, 0};
        warps[i] = inter;
    }
    bounding_box_t bathroomWarpBox = {2, 164, 32, 32};
    warp_t bathroomWarp = {bathroomWarpBox, &room_bathroom_loadRoom, 240, 176};
    warps[0] = bathroomWarp;

    setWarps(warps);
    setBoundingBoxes(boundingBoxes);
    setInteractables(interactables);
}

void advanceCutscene() {
    /*gfx_SetColor(COLOR_WHITE);
    gfx_SetTextFGColor(COLOR_BLACK);
    const char* string;
    switch(computerCutsceneState) {
        case 2: {
            string = "...";
            break;
        }
        case 3: {
            string = "You found me.";
            break;
        }
        case 4: {
            string = "Wait.";
            break;
        }
        case 5: {
            string = "Am i on a calculator?? what???";
            break;
        }
        case 6: {
            string = "oh... this is a tech demo isn't it?";
            break;
        }
        case 7: {
            string = "well i guess i'll say the line, just for you.";
            break;
        }
        case 8: {
            gfx_End();
            gfxActive = false;
            os_DrawStatusBar();
            os_SetCursorPos(4, 2);
            os_PutStrFull("You only have one shot.");
            break;
        }
        case 9: {
            initGfx();
            gfxActive = true;
            gfx_FillScreen(COLOR_LIGHT_BLUE);
            string = "that good enough for you??";
            gfx_SetTextFGColor(COLOR_BLACK);
            break;
        }
        case 10: {
            string = "god i hate gamers...";
            break;
        }
        case 11: {
            gfx_End();
            gfxActive = false;
            os_DrawStatusBar();
            os_SetCursorPos(1, 2);
            os_PutStrFull("HAPPY 5TH ANNIVERSARY,");
            os_SetCursorPos(2, 9);
            os_PutStrFull("ONESHOT!");
            os_SetCursorPos(5, 1);
            os_PutStrFull("A scuffed demo by glotch");
            os_SetCursorPos(7, 6);
            os_PutStrFull("@glotchalotch");
            break;
        }
    }
    if(computerCutsceneState >= 2 && computerCutsceneState != 8 && computerCutsceneState < 11) {
        gfx_FillRectangle(gfx_lcdWidth/2 - 140, gfx_lcdHeight/2 - 60, 280, 120);
        gfx_PrintStringXY(string, gfx_lcdWidth/2 - (gfx_GetStringWidth(string)/2), gfx_lcdHeight/2 - 5);
    }
    computerCutsceneState++;*/
}