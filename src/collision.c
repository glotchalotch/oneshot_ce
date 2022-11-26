#include <tice.h>
#include <graphx.h>
#include "collision.h"

bounding_box_t boundingBoxes[BBOX_ARR_SIZE];
interactable_t interactables[INTERACTABLE_ARR_SIZE];
warp_t warps[WARP_ARR_SIZE];

//TODO: it would probably wise to convert bboxes and interactables to pointers
void setBoundingBoxes(bounding_box_t boxes[BBOX_ARR_SIZE]) {
    for(int i = 0; i < BBOX_ARR_SIZE; i++) {
        boundingBoxes[i] = boxes[i];
    }
}

bounding_box_t* getBoundingBoxes() {
    return boundingBoxes;
}

void makeEmptyBoundingBoxArray(bounding_box_t* array) {
    for(int i = 0; i < BBOX_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        array[i] = bbox;
    }
}

void setInteractables(interactable_t interactableList[INTERACTABLE_ARR_SIZE]) {
    for(int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        interactables[i] = interactableList[i];
    }
}

interactable_t* getInteractables() {
    return interactables;
}

void makeEmptyInteractableArray(interactable_t* array) {
    for(int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        interactable_t inter = {bbox, NULL, NULL};
        array[i] = inter;
    }
}

void removeInteractable(uint8_t index) {
    interactable_t i = {{0, 0, 0, 0}, NULL, NULL};
    interactables[index] = i;
}

warp_t* getWarps() {
    return warps;
}

void makeEmptyWarpArray(warp_t* array) {
    for(int i = 0; i < WARP_ARR_SIZE; i++) {
        bounding_box_t bbox = {0, 0, 0, 0};
        warp_t inter = {bbox, NULL, 0, 0};
        array[i] = inter;
    }
}

void setWarps(warp_t warpList[WARP_ARR_SIZE]) {
    for(int i = 0; i < WARP_ARR_SIZE; i++) {
        warps[i] = warpList[i];
    }
}

void raycastInteractable(uint8_t direction, int spriteX, int spriteY, uint8_t spriteW, uint8_t spriteH) {
    for(unsigned int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        bool hit = false;
        short bX = interactables[i].boundingBox.x;
        short bY = interactables[i].boundingBox.y;
        short bW = interactables[i].boundingBox.width;
        short bH = interactables[i].boundingBox.height;
        if(bW != 0 && bH != 0) {
            int centerX = spriteX + (spriteW / 2);
            switch (direction)
            {
            case 3: // right
                if (gfx_CheckRectangleHotspot(centerX, spriteY + spriteH, 36, 1, bX, bY, bW, bH))
                    hit = true;
                break;
            case 2: // left
                if (gfx_CheckRectangleHotspot(centerX - 36, spriteY + spriteH, 36, 1, bX, bY, bW, bH))
                    hit = true;
                break;
            case 0: // down
                if (gfx_CheckRectangleHotspot(centerX, spriteY + spriteH, 1, 16, bX, bY, bW, bH))
                    hit = true;
                break;
            case 1: // up
                if (gfx_CheckRectangleHotspot(centerX, spriteY + spriteH - 16, 1, 16, bX, bY, bW, bH))
                    hit = true;
                break;
            }
            if (hit)
            {
                interactables[i].onHit(interactables[i].onHitArg);
                break;
            }
        }
        
    }
    
}

bool checkCollision(int tryX, int tryY, uint8_t spriteW, uint8_t spriteH) {
    bool blocked = false;
    for (int i = 0; i < BBOX_ARR_SIZE; i++)
    {
        short bX = boundingBoxes[i].x;
        short bY = boundingBoxes[i].y;
        short bW = boundingBoxes[i].width;
        short bH = boundingBoxes[i].height;
        //rect is height 1 so that the collision check effectively only checks feet so niko can get close to objects
        if (gfx_CheckRectangleHotspot(tryX, tryY + spriteH - 1, spriteW, 1, bX, bY, bW, bH))
        {
            blocked = true;
            break;
        }
    }
    return blocked;
}

bool justWarped = false;

void checkAndWarp(int* curX, int* curY, uint8_t spriteWidth, uint8_t spriteHeight) {
    warp_t* foundWarp = NULL;
    for(int i = 0; i < WARP_ARR_SIZE; i++) {
        if(gfx_CheckRectangleHotspot(*curX + (spriteWidth / 2), *curY + spriteHeight, 1, 1, warps[i].boundingBox.x, warps[i].boundingBox.y, warps[i].boundingBox.width, warps[i].boundingBox.height)) {
            foundWarp = &warps[i];
            if(!justWarped) {
                *curX = warps[i].destX;
                *curY = warps[i].destY;
                justWarped = true;
                warps[i].roomLoadFunction();
            }
        }
    }
    if(foundWarp == NULL) justWarped = false;
}