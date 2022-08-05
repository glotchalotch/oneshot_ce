#include <graphx.h>
#include <tice.h>
#include <compression.h>
#include <debug.h>

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

void windowCutscene() {
    static uint8_t windowCutsceneState = 0;
    if(inventory_getCurrentItem()->id == ITEM_HOUSE_REMOTE) {
        switch(windowCutsceneState) {
            case 0: {
                const char *dialogue[3] = {"", "[In the faint light,", "Niko can glimpse the face of the remote.]"};
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                setOnDialogueHide(&windowCutscene);
                windowCutsceneState++;
                break;
            }
            case 1: {
                const char *dialogue[3] = {"", "[All of the numbers except", "4, 6, 2, and 8 are missing.]"};
                colored_text_t textStruct1 = {2, 0, 1, COLOR_TRANSPARENT_GREEN};
                colored_text_t textStruct2 = {2, 3, 1, COLOR_LIGHT_BLUE};
                colored_text_t textStruct3 = {2, 6, 1, COLOR_YELLOW};
                colored_text_t textStruct4 = {2, 13, 1, COLOR_RED};
                setColoredText(0, &textStruct1);
                setColoredText(1, &textStruct2);
                setColoredText(2, &textStruct3);
                setColoredText(3, &textStruct4);
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                windowCutsceneState++;
                break;
            }
            case 2: {
                const char *dialogue[3] = {"", "[They're marked in bright colors.]", ""};
                showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
                windowCutsceneState = 0;
                setOnDialogueHide(NULL);
                break;
            }
        }
        
    } else {

    }
}

void remoteCutscene() {
    if(remoteCutsceneState == 0) {
        const char *dialogue[3] = {"", "[There's a TV remote here.]", ""};
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
        setOnDialogueHide(&remoteCutscene);
        remoteCutsceneState = 1;
    } else if(remoteCutsceneState == 1) {
        const char *dialogue[3] = {"", "[Niko picks it up.]", ""};
        showDialogue(dialogue, DIALOGUE_TYPE_IMPERSONAL);
        remoteCutsceneState = 2;
        
    } else if(remoteCutsceneState == 2) {
        static item_t remote = {ITEM_HOUSE_REMOTE, "remote"};
        inventory_addItem(&remote);
        gfx_Sprite(behind_remote, 262, 150);
        free(behind_remote);
        removeInteractable(1);
    }
}

void computerCutscene() {
    renderNiko = false;
    
}

void room_start_renderRoom() {
    //rendering the bg all at once puts it on the verge of running out of ram
    //so i have to juggle mem a bit like this
    //probably for the best anyway
    unzipScaleDrawSprite(room_house_start_bg1_compressed, room_house_start_bg1_width, room_house_start_bg1_height, 2, 0, 0);
    unzipScaleDrawSprite(room_house_start_bg2_compressed, room_house_start_bg2_width, room_house_start_bg2_height, 2, 80, 0);
    unzipScaleDrawSprite(room_house_start_bg3_compressed, room_house_start_bg3_width, room_house_start_bg3_height, 2, 160, 0);
    unzipScaleDrawSprite(room_house_start_bg4_compressed, room_house_start_bg4_width, room_house_start_bg4_height, 2, 240, 0);

    if(remoteCutsceneState < 1) {
        gfx_sprite_t *remote = gfx_MallocSprite(remote_width, remote_height);
        behind_remote = gfx_MallocSprite(remote_width, remote_height);
        gfx_GetSprite(behind_remote, 262, 150);
        zx7_Decompress(remote, remote_compressed);
        gfx_TransparentSprite(remote, 262, 150);
        free(remote);
    }
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

    bounding_box_t windowBox = {256, 80, 56, 52};
    interactables[2].boundingBox = windowBox;
    interactables[2].onHit = &windowCutscene;

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
        gfx_FillRectangle(GFX_LCD_WIDTH/2 - 140, GFX_LCD_HEIGHT/2 - 60, 280, 120);
        gfx_PrintStringXY(string, GFX_LCD_WIDTH/2 - (gfx_GetStringWidth(string)/2), GFX_LCD_HEIGHT/2 - 5);
    }
    computerCutsceneState++;*/
}