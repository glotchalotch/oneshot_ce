#include <graphx.h>
#include <compression.h>

#include "gfx/gfx.h"
#include "color.h"

void room_bathroom_renderRoom() {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle(0, 0, gfx_lcdWidth, gfx_lcdHeight);

    gfx_sprite_t* bg1 = gfx_MallocSprite(room_house_bathroom_bg1_width * 2, room_house_bathroom_bg1_height * 2);
    gfx_sprite_t* bg1_uncompressed = gfx_MallocSprite(room_house_bathroom_bg1_width, room_house_bathroom_bg1_height);
    zx7_Decompress(bg1_uncompressed, room_house_bathroom_bg1_compressed);
    gfx_ScaleSprite(bg1_uncompressed, bg1);
    free(bg1_uncompressed);
    gfx_Sprite(bg1, 40, 0);
    free(bg1);

    gfx_sprite_t* bg2 = gfx_MallocSprite(room_house_bathroom_bg2_width * 2, room_house_bathroom_bg2_height * 2);
    gfx_sprite_t* bg2_uncompressed = gfx_MallocSprite(room_house_bathroom_bg2_width, room_house_bathroom_bg2_height);
    zx7_Decompress(bg2_uncompressed, room_house_bathroom_bg2_compressed);
    gfx_ScaleSprite(bg2_uncompressed, bg2);
    free(bg2_uncompressed);
    gfx_Sprite(bg2, (room_house_bathroom_bg1_width * 2) + 40, 0);
    free(bg2);

    gfx_sprite_t* bg3 = gfx_MallocSprite(room_house_bathroom_bg3_width * 2, room_house_bathroom_bg3_height * 2);
    gfx_sprite_t* bg3_uncompressed = gfx_MallocSprite(room_house_bathroom_bg3_width, room_house_bathroom_bg3_height);
    zx7_Decompress(bg3_uncompressed, room_house_bathroom_bg3_compressed);
    gfx_ScaleSprite(bg3_uncompressed, bg3);
    free(bg3_uncompressed);
    gfx_Sprite(bg3, (room_house_bathroom_bg1_width * 2) + (room_house_bathroom_bg2_width * 2) + 40, 0);
    free(bg3);
}

void room_bathroom_loadRoom() {
    room_bathroom_renderRoom();
}