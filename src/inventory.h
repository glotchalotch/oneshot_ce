#include <tice.h>

#define ITEM_NONE 0
#define ITEM_HOUSE_REMOTE 1
#define ITEM_HOUSE_BRANCH 2
#define ITEM_HOUSE_ALCOHOL 3
#define ITEM_HOUSE_WETBRANCH 4
#define ITEM_HOUSE_LITBRANCH 5
#define ITEM_LIGHTBULB 6
#define ITEM_EMPTYBOTTLE 7
#define ITEM_HOUSE_BASEMENTKEY 8

#define INVENTORY_CURSORDIR_NONE 0
#define INVENTORY_CURSORDIR_DOWN 1
#define INVENTORY_CURSORDIR_UP 2
#define INVENTORY_CURSORDIR_LEFT 3
#define INVENTORY_CURSORDIR_RIGHT 4

typedef struct item {
    uint8_t id;
    const char* name;

    //tbd whether to add these
    //gfx_sprite_t sprite;
    //const char* description[x];
} item_t;

extern bool inventoryRendering;

void inventory_init();
void inventory_toggle();
void inventory_clean();
void inventory_moveCursor(uint8_t dir);
void inventory_renderInventory();
void inventory_selectHighlightedItem();
void inventory_addItem(item_t* item);
void inventory_removeItem(item_t* item);
item_t* inventory_getCurrentItem();