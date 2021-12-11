#include <graphx.h>
#include <tice.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "dialogue.h"
#include "main.h"
#include "collision.h"
#include "room_test.h"

uint8_t computerCutsceneState = 0;
bool inComputerCutscene = false;

void renderRoom() {
    //rendering the bg all at once puts it on the verge of running out of.. some kind of ram i havent figured out what yet 
    //so i have to juggle mem a bit like this
    //probably for the best anyway
    gfx_sprite_t* room_house_start_bg1 = gfx_MallocSprite(room_house_start_bg1_width * 2, room_house_start_bg1_height * 2);
    zx7_Decompress(room_house_start_bg1, room_house_start_bg1_compressed);
    gfx_ScaleSprite(room_house_start_bg1, room_house_start_bg1);
    gfx_Sprite(room_house_start_bg1, 0, 0);
    free(room_house_start_bg1);
    
    gfx_sprite_t* room_house_start_bg2 = gfx_MallocSprite(room_house_start_bg2_width * 2, room_house_start_bg2_height * 2);
    zx7_Decompress(room_house_start_bg2, room_house_start_bg2_compressed);
    gfx_ScaleSprite(room_house_start_bg2, room_house_start_bg2);
    gfx_Sprite(room_house_start_bg2, 160, 0);
    free(room_house_start_bg2);

    /*gfx_sprite_t* photo = gfx_MallocSprite(photo_width, photo_height);
    zx7_Decompress(photo, photo_compressed);
    gfx_sprite_t* computer_off = gfx_MallocSprite(computer_off_width, computer_off_height);
    zx7_Decompress(computer_off, computer_off_compressed);
    gfx_TransparentSprite(photo, 214, 96);
    gfx_TransparentSprite(computer_off, 118, 48);
    free(computer_off);
    free(photo);*/
}

void loadRoom() {
    renderRoom();

    bounding_box_t boundingBoxes[BBOX_ARR_SIZE];
    for(int i = 0; i < BBOX_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        boundingBoxes[i] = bbox;
    }
    bounding_box_t backWallBox = {66, 0, 254, 99};
    boundingBoxes[0] = backWallBox;

    bounding_box_t objBox = {129, 98, 121, 33};
    boundingBoxes[1] = objBox;

    bounding_box_t frontWallBox = {0, 66, 66, 69};
    boundingBoxes[2] = frontWallBox;

    interactable_t interactables[INTERACTABLE_ARR_SIZE];
    for(int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        interactable_t inter = {bbox, NULL, NULL};
        interactables[i] = inter;
    }
    bounding_box_t computerBox = {129, 86, 58, 46};
    interactables[0].boundingBox = computerBox;
    interactables[0].onHit = (void(*)(void*))&computerCutscene;

    /*interactables[1].boundingBox = boundingBoxes[1];
    interactables[1].onHit = (void(*)(void*))&computerCutscene;*/

    setBoundingBoxes(boundingBoxes);
    setInteractables(interactables);
}

void beginCutscene() {
    gfx_FillScreen(3);
    computerCutsceneState = 1;
    renderNiko = false;
    inComputerCutscene = true;
}

void advanceCutscene() {
    gfx_SetColor(2);
    gfx_SetTextFGColor(1);
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
            gfx_FillScreen(3);
            string = "that good enough for you??";
            gfx_SetTextFGColor(1);
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
    computerCutsceneState++;
}

void computerCutscene() {
    const char* offDialogue[3] = {"It's a computer!", "It's turned off.", ""};
    const char* onDialogue[3] = {"It turned on!", "", ""};
    if(computerCutsceneState == 0) {
        showDialogue(offDialogue);
    } else if(computerCutsceneState == 1) {
        showDialogue(onDialogue);
        gfx_sprite_t* computer_on = gfx_MallocSprite(computer_on_width, computer_on_height);
        zx7_Decompress(computer_on, computer_on_compressed);
        gfx_TransparentSprite(computer_on, 118, 48);
        setOnDialogueHide(beginCutscene);
    }
}

void photoCutscene() {
    const char* dialogue[3] = {"It's a framed photo of me", "turning on a computer!", "Hmm..."};
    showDialogue(dialogue);
    if(computerCutsceneState == 0) computerCutsceneState = 1;
}