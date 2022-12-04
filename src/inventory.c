#include <tice.h>
#include <graphx.h>
#include "inventory.h"
#include "gfx/gfx.h"
#include <compression.h>
#include "color.h"
#include "input.h"
#include "main.h"

#define INVENTORY_LENGTH 10
#define INVENTORY_COMBO_TABLE_SIZE 3
#define INVENTORY_CURSOR_NONESELECTED -1
#define INVENTORY_ADD_FAILED 255
#define INVENTORY_WIDTH GFX_LCD_WIDTH
#define INVENTORY_HEIGHT GFX_LCD_HEIGHT / 2

item_t* inventory[INVENTORY_LENGTH];
gfx_sprite_t* cursor;

int8_t selectedCursorPos = INVENTORY_CURSOR_NONESELECTED;
uint8_t cursorPos = 0;

bool inventoryRendering = false;

//reminder: don't try to free this null item
item_t nullItem = {
    ITEM_NONE,
    ""
};

uint8_t inventory_addItem(item_t* item) {
    // returns index in array of added item
    for(uint8_t i = 0; i < INVENTORY_LENGTH; i++) {
        if(inventory[i]->id == ITEM_NONE) {
            inventory[i] = item;
            if(selectedCursorPos > i) selectedCursorPos++;
            return i;
        }
    }
    return INVENTORY_ADD_FAILED;
}

void inventory_removeItem(uint8_t index) {
    bool itemRemoved = false;
    for(uint8_t i = 0; i < INVENTORY_LENGTH; i++) {
        if(!itemRemoved) {
            if(i == index) {
                free(inventory[i]);
                inventory[i] = &nullItem;
                itemRemoved = true;
            }
        } else {
            inventory[i - 1] = inventory[i];
            inventory[i] = &nullItem;
            if(selectedCursorPos == i) {
                selectedCursorPos = INVENTORY_CURSOR_NONESELECTED;
            } else if(selectedCursorPos > i) {
                selectedCursorPos--;
            }
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
        if(selectedCursorPos == i) {
            gfx_SetTextFGColor(COLOR_LIGHT_BLUE);
        } else gfx_SetTextFGColor(COLOR_WHITE);
        gfx_PrintStringXY(inventory[i]->name, ((i % 2) * (INVENTORY_WIDTH / 2)) + 15, ((i - (i % 2)) * 10) + 10);
    }
    inventory_moveCursor(INVENTORY_CURSORDIR_NONE);
}

typedef struct item_combination_table {
    uint8_t combineItemId;
    item_t* resultItem;
    void* cutscenePointer;
} item_combination_table_t;

item_combination_table_t* inventory_getItemCombinationTable(uint8_t item) {
    static item_combination_table_t tables[INVENTORY_COMBO_TABLE_SIZE];
    for(int i = 0; i < INVENTORY_COMBO_TABLE_SIZE; i++) {
        tables[i].combineItemId = ITEM_NONE;
        tables[i].resultItem = &nullItem;
        tables[i].cutscenePointer = NULL;
    }
    switch(item) {
        case ITEM_HOUSE_BRANCH: {
            tables[0].combineItemId = ITEM_HOUSE_ALCOHOL;
            static item_t wetBranch = {ITEM_HOUSE_WETBRANCH, "wet branch"};
            tables[0].resultItem = &wetBranch;
            tables[0].cutscenePointer = NULL; // todo
            break;
        }
    }
    return tables;
}

void inventory_combineItems(uint8_t index1, uint8_t index2) {
    uint8_t a = inventory[index1]->id;
    uint8_t b = inventory[index2]->id;
    if(b < a) {
        // lower number always needs to come first such that i only have to write combinations once
        uint8_t temp = a;
        a = b;
        b = temp;
    }
    item_combination_table_t* comboTable = inventory_getItemCombinationTable(a);
    for(int i = 0; i < INVENTORY_COMBO_TABLE_SIZE; i++) {
        if(comboTable[i].combineItemId == b) {
            if(index2 > index1) index2--; //pre-emptively preparing for indices shifting when removing index1
            inventory_removeItem(index1);
            inventory_removeItem(index2);
            uint8_t addedIndex = inventory_addItem(comboTable[i].resultItem);
            if(addedIndex != INVENTORY_ADD_FAILED) {
                selectedCursorPos = addedIndex;
                cursorPos = addedIndex;
            }
        }
    }
}

void inventory_selectHighlightedItem() {
    if(selectedCursorPos != cursorPos && inventory[cursorPos] != &nullItem) {
        if(selectedCursorPos == INVENTORY_CURSOR_NONESELECTED) selectedCursorPos = cursorPos;
        else {
            inventory_combineItems(selectedCursorPos, cursorPos);
        }
    } else {
        selectedCursorPos = INVENTORY_CURSOR_NONESELECTED;
    }
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
    if(selectedCursorPos != INVENTORY_CURSOR_NONESELECTED) {
        return inventory[selectedCursorPos];
    } else return NULL;
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