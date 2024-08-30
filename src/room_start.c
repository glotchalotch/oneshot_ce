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
#include "input.h"
#include "room_livingroom.h"
#include "tile.h"

uint8_t computerCutsceneState = 0;
bool inComputerCutscene = false;
uint8_t remoteCutsceneState = 0;
char curPassword[4] = {'0', '0', '0', '0'};
uint8_t curPasswordIndex = 0;

gfx_sprite_t* behind_remote;

void computerCutscene(); // c is so silly i love having to declare functions in order!

void room_start_unloadRoom() {
    if(remoteCutsceneState < 2) free(behind_remote);
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

void room_start_renderRoom() {
    //rendering the bg all at once puts it on the verge of running out of ram
    //so i have to juggle mem a bit like this
    //probably for the best anyway
    /*unzipScaleDrawSprite(room_house_start_bg1_compressed, room_house_start_bg1_width, room_house_start_bg1_height, 2, 0, 0);
    unzipScaleDrawSprite(room_house_start_bg2_compressed, room_house_start_bg2_width, room_house_start_bg2_height, 2, 80, 0);
    unzipScaleDrawSprite(room_house_start_bg3_compressed, room_house_start_bg3_width, room_house_start_bg3_height, 2, 160, 0);
    unzipScaleDrawSprite(room_house_start_bg4_compressed, room_house_start_bg4_width, room_house_start_bg4_height, 2, 240, 0);*/

    gfx_FillScreen(COLOR_BLACK);

    uint8_t map[70] = {58, 58, 84, 85, 85, 85, 85, 85, 105, 106,
        58, 58, 92, 93, 93, 93, 93, 93, 113, 114,
        3, 4, 93, 93, 93, 93, 93, 93, 121, 122,
        6, 7, 89, 90, 91, 89, 90, 91, 89, 90,
        21, 22, 97, 98, 109, 110, 110, 110, 111, 98,
        104, 112, 89, 90, 125, 126, 126, 126, 127, 90,
        58, 58, 58, 58, 58, 58, 58, 96, 58, 58};

    drawBgTilemap(map, 7, 10, 0, 2);

    uint8_t map2[32] = {83, 83, 83, 83, 61, 62, 83, 83,
        19, 20, 16, 17, 69, 70, 83, 83,
        27, 28, 24, 25, 77, 78, 83, 83,
        35, 36, 83, 83, 83, 83, 83, 83};

    drawFgTilemap(map2, 4, 8, 30, 22);

    uint8_t map3[80] = {4, 4, 0, 1, 1, 1, 1, 1, 1, 2,
        4, 4, 3, 4, 4, 4, 4, 4, 4, 4,
        0, 1, 4, 4, 4, 4, 4, 4, 4, 5,
        3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        1, 1, 1, 1, 1, 1, 2, 4, 0, 1,
        4, 4, 4, 4, 4, 4, 4, 1, 4, 4};

    drawOutlineTilemap(map3, 8, 10, 0, 2);

    gfx_SetColor(COLOR_OUTLINE);
    gfx_SetPixel(32, 34);
    gfx_SetPixel(111, 114);
    gfx_SetPixel(128, 114);
    
    expandQuadrant(3);
    expandQuadrant(2);
    expandQuadrant(1);
    expandQuadrant(0);

    if(remoteCutsceneState < 2) {
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
    makeEmptyBoundingBoxArray(boundingBoxes);
    
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
    makeEmptyInteractableArray(interactables);

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
    makeEmptyWarpArray(warps);

    bounding_box_t bathroomWarpBox = {2, 164, 32, 32};
    warp_t bathroomWarp = {bathroomWarpBox, &room_bathroom_loadRoom, 196, 152};
    warps[0] = bathroomWarp;

    if(computerCutsceneState == 18) {
        warp_t livingRoomWarp = {{226, 196, 32, 32}, &room_livingroom_loadRoom, 55, 46};
        warps[1] = livingRoomWarp;
    }

    setWarps(warps);
    setBoundingBoxes(boundingBoxes);
    setInteractables(interactables);

    setUnloadRoomFunction(&room_start_unloadRoom);
}

void setCurPasswordIndex(uint8_t index) {
    curPasswordIndex = index;
    if(curPasswordIndex > 250) curPasswordIndex = 3; // for underflow
    if(curPasswordIndex > 3) curPasswordIndex = 0; // for max cap
}

void typeNumber(char number) {
    uint8_t x = 10 + (20 * curPasswordIndex);
    gfx_SetTextXY(x, 20);
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(x - 2, 18, 20, 20);
    uint8_t textColor = COLOR_WHITE;
    switch(curPasswordIndex) {
        case 0:
            textColor = COLOR_LIGHT_BLUE;
            break;
        case 1:
            textColor = COLOR_TRANSPARENT_GREEN;
            break;
        case 2:
            textColor = COLOR_RED;
            break;
        case 3:
            textColor = COLOR_YELLOW;
            break;
    }
    gfx_SetTextFGColor(textColor);
    gfx_PrintChar(number);
    curPassword[curPasswordIndex] = number;
    setCurPasswordIndex(curPasswordIndex + 1);
    gfx_SetColor(COLOR_WHITE);
    gfx_Rectangle(10 + (20 * curPasswordIndex) - 2, 18, 11, 20);
}

void computerCutsceneInputHandler(sk_key_t key) {
    // this feels hopelessly inefficient but there doesn't seem to be an easier way to convert num keys to actual nums
    if(key == sk_Enter) computerCutscene();
    if(computerCutsceneState == 1) {
        switch (key)
        {
        case sk_0:
            typeNumber('0');
            break;
        case sk_1:
            typeNumber('1');
            break;
        case sk_2:
            typeNumber('2');
            break;
        case sk_3:
            typeNumber('3');
            break;
        case sk_4:
            typeNumber('4');
            break;
        case sk_5:
            typeNumber('5');
            break;
        case sk_6:
            typeNumber('6');
            break;
        case sk_7:
            typeNumber('7');
            break;
        case sk_8:
            typeNumber('8');
            break;
        case sk_9:
            typeNumber('9');
            break;
        case sk_Left:
            gfx_SetColor(COLOR_BLACK);
            gfx_Rectangle(10 + (20 * curPasswordIndex) - 2, 18, 11, 20);
            setCurPasswordIndex(curPasswordIndex - 1);
            gfx_SetColor(COLOR_WHITE);
            gfx_Rectangle(10 + (20 * curPasswordIndex) - 2, 18, 11, 20);
            break;
        case sk_Right:
            gfx_SetColor(COLOR_BLACK);
            gfx_Rectangle(10 + (20 * curPasswordIndex) - 2, 18, 11, 20);
            setCurPasswordIndex(curPasswordIndex + 1);
            gfx_SetColor(COLOR_WHITE);
            gfx_Rectangle(10 + (20 * curPasswordIndex) - 2, 18, 11, 20);
            break;
        }
    }
    
}

void computerCutscene() {
    const char* string;
    bool blankFlag = false;
    switch(computerCutsceneState) {
        case 0:
            curPasswordIndex = 0;
            for(int i = 0; i < 4; i++) {
                curPassword[i] = '0';
            }
            setRenderNiko(false);
            gfx_FillScreen(COLOR_BLACK);
            gfx_SetTextFGColor(COLOR_WHITE);
            gfx_PrintStringXY("Input Password:", 0, 0);
            for(int i = 0; i < 4; i++) {
                typeNumber('0');
            }
            
            //gfx_Rectangle(0, 0, 16, 16);
            setInputHandler(computerCutsceneInputHandler);
            computerCutsceneState++;
            break;
        case 1:
            gfx_SetColor(COLOR_BLACK);
            gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);
            if(curPassword[0] == '6' && curPassword[1] == '4' && curPassword[2] == '8' && curPassword[3] == '2') {
                gfx_SetTextFGColor(COLOR_TRANSPARENT_GREEN);
                gfx_PrintStringXY("Access Granted.", 0, 0);
                computerCutsceneState = 3;
            } else {
                gfx_SetTextFGColor(COLOR_RED);
                gfx_PrintStringXY("Access Denied.", 0, 0);
                computerCutsceneState = 2;
            }
            break;
        case 2:
            room_start_renderRoom();
            setRenderNiko(true);
            setInputHandler(defaultInputHandler);
            computerCutsceneState = 0;
            break;
        case 3:
            gfx_FillScreen(COLOR_LIGHT_BLUE);
            gfx_SetColor(COLOR_WHITE);
            gfx_SetTextFGColor(COLOR_BLACK);
            gfx_SetTextBGColor(COLOR_WHITE);
            computerCutsceneState++;
            blankFlag = true;
            break;
        case 4: 
            string = "...";
            break;
        case 5: 
            string = "You found me.";
            break;
        case 6: 
            string = "...Why?";
            break;
        case 7:
            string = "You're already too late.";
            break;
        case 8:
            string = "Not much of the world remains.";
            break;
        case 9:
            string = "This will be apparent once you go outside.";
            break;
        case 10:
            string = "This place was never worth saving.";
            break;
        case 11:
            string = "...Do you still want to try?";
            break;
        case 12:
            string = "Then, remember this:";
            break;
        case 13:
            string = "Your actions here will affect Niko.";
            break;
        case 14:
            string = "Your \"mission\" is to help Niko leave.";
            break;
        case 15:
            string = "And most importantly...";
            break;
        case 16:
            gfx_End();
            setGfxActive(false);
            os_DrawStatusBar();
            os_SetCursorPos(4, 2);
            os_PutStrFull("You only have one shot.");
            computerCutsceneState++;
            break;
        case 17:
            initGfx();
            setGfxActive(true);
            room_start_renderRoom();
            setRenderNiko(true);
            computerCutsceneState++;
            const char* endDialogue[3] = {"", "[Niko hears the sound of a door unlocking.]", ""};
            warp_t kitchenWarp = {{226, 196, 32, 32}, &room_livingroom_loadRoom, 55, 46};
            warp_t* warps = getWarps();
            warps[1] = kitchenWarp;
            setWarps(warps);
            setOnDialogueHide(NULL); // i thought this was implicit but after rewriting inventory logic, not having this line caused some kind of game-crashing item dupe if items were in the inventory during this part. okay!!!
            showDialogue(endDialogue, DIALOGUE_TYPE_IMPERSONAL);
            break;
    }
    if(computerCutsceneState >= 4 && computerCutsceneState < 16 && !blankFlag) {
        gfx_FillRectangle(GFX_LCD_WIDTH/2 - 140, GFX_LCD_HEIGHT/2 - 60, 280, 120);
        gfx_PrintStringXY(string, GFX_LCD_WIDTH/2 - (gfx_GetStringWidth(string)/2), GFX_LCD_HEIGHT/2 - 5);
        computerCutsceneState++;
    }
}