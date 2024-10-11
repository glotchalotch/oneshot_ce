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

#define INVENTORY_ADD_FAILED 255
#define INVENTORY_ITEM_NOT_FOUND 255

typedef struct item {
    uint8_t id;
    const char* name;

    //tbd whether to add these
    //gfx_sprite_t sprite;
    //const char* description[x];
} item_t;

void inventory_init();
void inventory_toggle();
void inventory_moveCursor(uint8_t dir);
void inventory_renderInventory();
void inventory_selectItem(uint8_t index);
void inventory_selectHighlightedItem();
uint8_t inventory_addItem(item_t* item);
void inventory_removeItem(uint8_t index);
uint8_t inventory_getItemIndex(item_t* item);
uint8_t inventory_getItemIndexById(uint8_t id);
item_t* inventory_getCurrentItem();
void inventory_inputHandler(sk_key_t key);
bool inventory_isInventoryRendering();
bool inventory_isInitialized(void);