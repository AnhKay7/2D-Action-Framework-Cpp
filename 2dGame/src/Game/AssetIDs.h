#pragma once

// ===== Texture =====
#define ID_TEX_PLAYER_SMALL         10
#define ID_TEX_BRICK                50
#define ID_TEX_TILESET				60
#define ID_TEX_STAGE_TILES			61
#define ID_TEX_BBOX                 90

#pragma region SPRITE
// ===== Sprite ===== 
#define ID_SPRITE_PLAYER_SMALL      10000
#define ID_SPRITE_BRICK             50000
// ===== Sprite cắt từ tileset: ID = ID_SPRITE_TILE_BASE + số ô =====
#define ID_SPRITE_TILE_BASE     60000
#define ID_SPRITE_BRICK_PIECE   61000
#define ID_SPRITE_STAGE_BASE	62000
// ===== Sprite object id ===== for stage tiles
#define TILE_QBLOCK_1     367
#define TILE_QBLOCK_2     368
#define TILE_QBLOCK_3     399
#define TILE_QBLOCK_4     400
#define TILE_QBLOCK_EMPTY 302
#define TILE_BRICK        401
#define TILE_COIN_1       333
#define TILE_COIN_2       334
#define TILE_COIN_3       335
#define TILE_GROW_ITEM    367
#pragma endregion

#pragma region ANIMATION
// ===== PLAYER =====
#define ID_ANI_PLAYER_IDLE          100
#define ID_ANI_PLAYER_WALK          101
#define ID_ANI_PLAYER_RUN           102
#define ID_ANI_PLAYER_SKID          103
#define ID_ANI_PLAYER_JUMP          104
#define ID_ANI_PLAYER_FALL          105

// ===== BLOCK & ITEM =====
#define ID_ANI_QBLOCK           200
#define ID_ANI_QBLOCK_EMPTY     201
#define ID_ANI_BRICK            202
#define ID_ANI_COIN             203
#define ID_ANI_COIN_POP         204
#define ID_ANI_GROW_ITEM        205
#define ID_ANI_BRICK_PIECE      206
#pragma endregion

#pragma region TEX_PATH
#define TEX_PATH_PLAYER_SMALL   L"assets\\test\\player_small_placeholder.png"
#define TEX_PATH_BRICK          L"assets\\tiles\\brick.png"
#define TEX_PATH_BBOX           L"assets\\debug\\bbox.png"
#define TEX_PATH_STAGE_TILES	L"assets\\tiles\\Stage_Tiles_Plains1.png"
// Đường dẫn map là chuỗi thường (không có L) vì đọc bằng std::ifstream
#define MAP_PATH_LEVEL1			"assets\\tiles\\map-1.1.tmj"
#pragma endregion