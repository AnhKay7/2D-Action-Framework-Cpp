#pragma once

// ===== Texture =====
#define ID_TEX_PLAYER_IDLE			10
#define ID_TEX_PLAYER_RUN			11
#define ID_TEX_PLAYER_ATTACK1		12
#define ID_TEX_PLAYER_JUMP			13
#define ID_TEX_PLAYER_FALL			14
#define ID_TEX_PLAYER_DASH			15

#define ID_TEX_BRICK				50
#define ID_TEX_BBOX					90

// ===== Sprite ===== (mỗi nhóm một dải nghìn cho khỏi đụng nhau)
#define ID_SPRITE_PLAYER_IDLE		10000   
#define ID_SPRITE_PLAYER_RUN		10100   
#define ID_SPRITE_PLAYER_ATTACK1	10200
#define ID_SPRITE_PLAYER_JUMP		10300
#define ID_SPRITE_PLAYER_FALL		10400
#define ID_SPRITE_PLAYER_DASH		10500

#define ID_SPRITE_BRICK				50000

// ===== Animation =====
#define ID_ANI_PLAYER_IDLE			100
#define ID_ANI_PLAYER_RUN			101
#define ID_ANI_PLAYER_ATTACK1		102
#define ID_ANI_PLAYER_JUMP			103
#define ID_ANI_PLAYER_FALL			104
#define ID_ANI_PLAYER_DASH			105

// ===== Đường dẫn =====
#define TEX_PATH_PLAYER_IDLE     L"assets\\player\\Idle.png"
#define TEX_PATH_PLAYER_RUN      L"assets\\player\\Run.png"
#define TEX_PATH_PLAYER_ATTACK1  L"assets\\player\\Attack1.png"
#define TEX_PATH_PLAYER_JUMP     L"assets\\player\\Jump.png"
#define TEX_PATH_PLAYER_FALL     L"assets\\player\\Fall.png"
#define TEX_PATH_PLAYER_DASH      L"assets\\player\\Dash.png"

#define TEX_PATH_BRICK           L"assets\\tiles\\brick.png"
#define TEX_PATH_BBOX            L"assets\\debug\\bbox.png"