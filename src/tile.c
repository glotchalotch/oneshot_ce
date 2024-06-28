#include <graphx.h>
#include <tice.h>
#include <compression.h>
#include "gfx/gfx.h"

void drawFgTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y) {
    gfx_sprite_t *tilesFg[tileset_house_fg_num_tiles];

    for(uint8_t i = 0; i < tileset_house_fg_num_tiles; i++) {
        gfx_sprite_t* tmp = gfx_MallocSprite(16, 16);
        zx7_Decompress(tmp, tileset_house_fg_tiles_compressed[i]);
        tilesFg[i] = tmp;
    }

    gfx_tilemap_t map = {
        tilemap, 
        tilesFg, 
        16,
        16,
        rows,
        cols, 
        gfx_tile_16_pixel,
        gfx_tile_16_pixel,
        rows,
        cols,
        y,
        x
    };

    gfx_TransparentTilemap(&map, 0, 0);

    for(uint8_t i = 0; i < tileset_house_fg_num_tiles; i++) {
        free(tilesFg[i]);
    }
}

void drawBgTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y) {
    gfx_sprite_t *tilesBg[tileset_house_bg_num_tiles];

    for(uint8_t i = 0; i < tileset_house_bg_num_tiles; i++) {
        gfx_sprite_t* tmp = gfx_MallocSprite(16, 16);
        zx7_Decompress(tmp, tileset_house_bg_tiles_compressed[i]);
        tilesBg[i] = tmp;
    }

    gfx_tilemap_t map = {
        tilemap, 
        tilesBg, 
        16,
        16,
        rows,
        cols, 
        gfx_tile_16_pixel,
        gfx_tile_16_pixel,
        rows,
        cols,
        y,
        x
    };

    gfx_TransparentTilemap(&map, 0, 0);

    for(uint8_t i = 0; i < tileset_house_bg_num_tiles; i++) {
        free(tilesBg[i]);
    }
}

void drawOutlineTilemap(uint8_t* tilemap, uint8_t rows, uint8_t cols, uint8_t x, uint8_t y) {
    gfx_sprite_t *tilesOutline[tileset_house_outline_num_tiles];

    for(uint8_t i = 0; i < tileset_house_outline_num_tiles; i++) {
        gfx_sprite_t* tmp = gfx_MallocSprite(16, 16);
        zx7_Decompress(tmp, tileset_house_outline_tiles_compressed[i]);
        tilesOutline[i] = tmp;
    }

    gfx_tilemap_t map = {
        tilemap, 
        tilesOutline, 
        16,
        16,
        rows,
        cols, 
        gfx_tile_16_pixel,
        gfx_tile_16_pixel,
        rows,
        cols,
        y,
        x
    };

    gfx_TransparentTilemap(&map, 0, 0);

    for(uint8_t i = 0; i < tileset_house_outline_num_tiles; i++) {
        free(tilesOutline[i]);
    }
}

void expandQuadrant(uint8_t q) {
    // quadrant from 0-3
    gfx_sprite_t *quadrant = gfx_MallocSprite(GFX_LCD_WIDTH / 8, GFX_LCD_HEIGHT / 2);
    gfx_sprite_t *quadrant_scaled = gfx_MallocSprite(GFX_LCD_WIDTH / 4, GFX_LCD_HEIGHT);
    gfx_GetSprite(quadrant, GFX_LCD_WIDTH * q / 8, 0);
    gfx_ScaleSprite(quadrant, quadrant_scaled);
    gfx_Sprite(quadrant_scaled, GFX_LCD_WIDTH * q / 4, 0);
    free(quadrant);
    free(quadrant_scaled);
}