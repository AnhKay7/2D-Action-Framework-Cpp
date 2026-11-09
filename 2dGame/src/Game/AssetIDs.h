#pragma once

// ===== Texture =====
#define ID_TEX_PLAYER_IDLE     10
#define ID_TEX_PLAYER_RUN      11
#define ID_TEX_PLAYER_ATTACK1  12
#define ID_TEX_BRICK           50
#define ID_TEX_BBOX            90

// ===== Sprite ===== (mỗi nhóm một dải nghìn cho khỏi đụng nhau)
// 1000-1999: Idle, 2000-2999: Run, 3000-3999: Attack1 (do AddStrip sinh ra)
#define ID_SPRITE_BRICK        50000

#define ID_SPRITE_PLAYER_IDLE    10000   
#define ID_SPRITE_PLAYER_RUN     10100   
#define ID_SPRITE_PLAYER_ATTACK1 10200

// ===== Animation =====
#define ID_ANI_PLAYER_IDLE     100
#define ID_ANI_PLAYER_RUN      101
#define ID_ANI_PLAYER_ATTACK1  102

// ===== Đường dẫn =====
#define TEX_PATH_PLAYER_IDLE     L"assets\\player\\Idle.png"
#define TEX_PATH_PLAYER_RUN      L"assets\\player\\Run.png"
#define TEX_PATH_PLAYER_ATTACK1  L"assets\\player\\Attack1.png"
#define TEX_PATH_BRICK           L"assets\\tiles\\brick.png"
#define TEX_PATH_BBOX            L"assets\\debug\\bbox.png"