#include <tice.h>
#include <graphx.h>
#include "collision.h"

bounding_box_t boundingBoxes[BBOX_ARR_SIZE];
interactable_t interactables[INTERACTABLE_ARR_SIZE];

//TODO: it would probably wise to convert bboxes and interactables to pointers
void setBoundingBoxes(bounding_box_t boxes[BBOX_ARR_SIZE]) {
    for(int i = 0; i < BBOX_ARR_SIZE; i++) {
        boundingBoxes[i] = boxes[i];
    }
}

bounding_box_t* getBoundingBoxes() {
    return boundingBoxes;
}

void setInteractables(interactable_t interactableList[INTERACTABLE_ARR_SIZE]) {
    for(int i = 0; i < INTERACTABLE_ARR_SIZE; i++) {
        interactables[i] = interactableList[i];
    }
}

interactable_t* getInteractables() {
    return interactables;
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
                if (gfx_CheckRectangleHotspot(centerX, spriteY, 36, 1, bX, bY, bW, bH))
                    hit = true;
                break;
            case 2: // left
                if (gfx_CheckRectangleHotspot(centerX - 36, spriteY, 36, 1, bX, bY, bW, bH))
                    hit = true;
                break;
            case 0: // down
                if (gfx_CheckRectangleHotspot(centerX, spriteY - 16, 1, 16, bX, bY, bW, bH))
                    hit = true;
                break;
            case 1: // up
                if (gfx_CheckRectangleHotspot(centerX, spriteY, 1, 16, bX, bY, bW, bH))
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