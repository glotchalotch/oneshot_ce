#include <tice.h>
#include <graphx.h>
#include "inventory.h"
#include "gfx/gfx.h"
#include <compression.h>
#include "color.h"
#include "input.h"
#include "main.h"

#define INVENTORY_LENGTH 10
#define INVENTORY_WIDTH GFX_LCD_WIDTH
#define INVENTORY_HEIGHT GFX_LCD_HEIGHT / 2

item_t* inventory[INVENTORY_LENGTH];
gfx_sprite_t* cursor;

item_t* currentItem = NULL;
uint8_t cursorPos = 0;

bool inventoryRendering = false;

//reminder: don't try to free this null item
item_t nullItem = {
    ITEM_NONE,
    ""
};

void inventory_addItem(item_t* item) {
    for(int i = 0; i < INVENTORY_LENGTH; i++) {
        if(inventory[i]->id == ITEM_NONE) {
            inventory[i] = item;
            break;
        }
    }
}

void inventory_removeItem(item_t* item) {
    bool itemRemoved = false;
    for(int i = 0; i < INVENTORY_LENGTH; i++) {
        if(!itemRemoved) {
            if(inventory[i] == item) {
                free(inventory[i]);
                inventory[i] = &nullItem;
                itemRemoved = true;
            }
        } else {
            inventory[i - 1] = inventory[i];
            inventory[i] = &nullItem;
        }
    }
}

void inventory_init() {
    for(int i = 0; i < INVENTORY_LENGTH; i++) {
        inventory[i] = &nullItem;
    }
}

void inventory_clean() {
    for(int i = 0; i < INVENTORY_LENGTH; i++) {
        if(inventory[i] != &nullItem) free(inventory[i]);
    }
}

void inventory_moveCursor(uint8_t dir) {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle((cursorPos % 2) * (INVENTORY_WIDTH / 2), ((cursorPos - (cursorPos%2)) * 10) + 10, cursor_width, cursor_height);
    uint8_t newCursorPos = cursorPos;
    switch(dir) {
        case INVENTORY_CURSORDIR_DOWN:
            if(cursorPos != INVENTORY_LENGTH - 2) newCursorPos += 2;
            break;
        case INVENTORY_CURSORDIR_UP:
            if(cursorPos != 1) newCursorPos -= 2;
            break;
        case INVENTORY_CURSORDIR_LEFT:
            if(cursorPos % 2 == 1) newCursorPos -= 1;
            break;
        case INVENTORY_CURSORDIR_RIGHT:
            if(cursorPos % 2 == 0) newCursorPos += 1;
            break;
    }
    if(newCursorPos > 250) newCursorPos = 0; //accounting for integer underflow when going below 0
    if(newCursorPos > INVENTORY_LENGTH - 1 && newCursorPos < 250) newCursorPos = INVENTORY_LENGTH - 1;
    if(inventory[newCursorPos] != &nullItem) cursorPos = newCursorPos;
    gfx_TransparentSprite(cursor, (cursorPos % 2) * (INVENTORY_WIDTH / 2), ((cursorPos - (cursorPos%2)) * 10) + 10);
}

void inventory_renderInventory() {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT);
    for (int i = 0; i < INVENTORY_LENGTH; i++)
    {
        if(currentItem == inventory[i]) {
            gfx_SetTextFGColor(COLOR_LIGHT_BLUE);
        } else gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(inventory[i]->name, ((i % 2) * (INVENTORY_WIDTH / 2)) + 15, ((i - (i % 2)) * 10) + 10);
    }
    inventory_moveCursor(INVENTORY_CURSORDIR_NONE);
}

void inventory_selectHighlightedItem() {
    if(currentItem != inventory[cursorPos] && inventory[cursorPos] != &nullItem) {
        currentItem = inventory[cursorPos];
    } else currentItem = NULL;
}

void inventory_toggle() {
    static gfx_sprite_t* behind_inventory1;
    static gfx_sprite_t* behind_inventory2;
    if(!inventoryRendering) {
        //width cant be >255 because uint8, same as dialogue
        //also i had to make the split half and half because memory spills over if i don't now?
        //this definitely isnt indicative of any overarching structural weaknesses in my programming
        behind_inventory1 = gfx_MallocSprite(INVENTORY_WIDTH / 2, INVENTORY_HEIGHT);
        behind_inventory2 = gfx_MallocSprite(INVENTORY_WIDTH / 2, INVENTORY_HEIGHT);
        gfx_GetSprite(behind_inventory1, 0, 0);
        gfx_GetSprite(behind_inventory2, INVENTORY_WIDTH / 2, 0);
        cursor = gfx_MallocSprite(cursor_width, cursor_height);
        zx7_Decompress(cursor, cursor_compressed);
        inventory_renderInventory();
        inventoryRendering = true;
        setInputHandler(inventory_inputHandler);
    } else {
        gfx_Sprite(behind_inventory1, 0, 0);
        gfx_Sprite(behind_inventory2, INVENTORY_WIDTH / 2, 0);
        free(behind_inventory1);
        free(behind_inventory2);
        free(cursor);
        inventoryRendering = false;
        setInputHandler(defaultInputHandler);
    }
}

item_t* inventory_getCurrentItem() {
    return currentItem;
}

bool inventory_isInventoryRendering() {
    return inventoryRendering;
}

void inventory_inputHandler(sk_key_t key) {
    switch(key) {
        case sk_Down:
            inventory_moveCursor(INVENTORY_CURSORDIR_DOWN);
            break;
        case sk_Up:
            inventory_moveCursor(INVENTORY_CURSORDIR_UP);
            break;
        case sk_Left:
            inventory_moveCursor(INVENTORY_CURSORDIR_LEFT);
            break;
        case sk_Right:
            inventory_moveCursor(INVENTORY_CURSORDIR_RIGHT);
            break;
        case sk_Enter:
            inventory_selectHighlightedItem();
            break;
        case sk_Add:
            inventory_toggle();
    }
}