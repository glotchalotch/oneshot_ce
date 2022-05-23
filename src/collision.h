#define BBOX_ARR_SIZE 10
#define INTERACTABLE_ARR_SIZE 10

typedef struct bounding_box {
    short x;
    short y;
    short width;
    short height;
} bounding_box_t;

typedef struct interactable {
    bounding_box_t boundingBox;
    void (*onHit)(void*);
    void* onHitArg;
} interactable_t;

void setBoundingBoxes(bounding_box_t boxes[BBOX_ARR_SIZE]);
bounding_box_t* getBoundingBoxes();

void setInteractables(interactable_t interactableList[INTERACTABLE_ARR_SIZE]);
interactable_t* getInteractables();
void removeInteractable(uint8_t index);

void raycastInteractable(uint8_t direction, int spriteX, int spriteY, uint8_t spriteW, uint8_t spriteH);
bool checkCollision(int tryX, int tryY, uint8_t spriteW, uint8_t spriteH);