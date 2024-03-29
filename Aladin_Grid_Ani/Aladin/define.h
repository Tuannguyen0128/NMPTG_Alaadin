#pragma once

//Main
#define WINDOW_CLASS_NAME L"AladinGame"
#define MAIN_WINDOW_TITLE L"AladinGame"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 255)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 250

#define MAX_FRAME_RATE 120


//*************************************
//Sprites
/*
	Aladin: 1xx
	10x: idle
	11x: Run
	12x: climp
	13x: jump
	14x: attack
	15x: jump attack
	16x: throw
	17x: sit attack
	18x
	19x


*/
//Ani
/*
	Aladin:1xx voi xx tu 1 den 15
*/

//Aladin
#define Aladin_GRAVITY 0.0011f//0.001f
#define Aladin_SPEED 0.1f
#define Aladin_CLIMB_SPEED 0.05f
#define Aladin_JUMP_FORCE 0.35f//0.3f
#define Aladin_STATE_IDLE			100
#define Aladin_STATE_RUN_RIGHT	110
#define Aladin_STATE_RUN_LEFT	111
#define Aladin_STATE_JUMP	120
#define Aladin_STATE_DOWN 130 //Co the la leo xuong hoac ngoi xuong
#define Aladin_STATE_HIT 140
#define Aladin_STATE_UP 150
#define Aladin_STATE_ONWALL 160
#define Aladin_STATE_ATTACKED 170

#define Aladin_ANI_BASE_NUMBER 100
#define Aladin_ANI_IDLE_RIGHT 0		//OK
#define Aladin_ANI_RUN_RIGHT 1		//
#define Aladin_ANI_IDLE_LEFT 2		//
#define Aladin_ANI_RUN_LEFT 3		//
#define Aladin_ANI_CLIMB_RIGHT 4		//
#define Aladin_ANI_CLIMB_LEFT 5		//
#define Aladin_ANI_JUMP_RIGHT 6		//
#define Aladin_ANI_JUMP_LEFT 7		//
#define Aladin_ANI_ATTACK_RIGHT 8	//
#define Aladin_ANI_ATTACK_LEFT 9		//
#define Aladin_ANI_JUMPATTACK_RIGHT 10	//??? Co roi
#define Aladin_ANI_JUMPATTACK_LEFT 11	//??? Co roi sao con them cai nay???
#define Aladin_ANI_THROW_RIGHT 12	//Nem phi tieu
#define Aladin_ANI_THROW_LEFT 13		//
#define Aladin_ANI_SITATTACK_RIGHT 14	//
#define Aladin_ANI_SITATTACK_LEFT 15		//
#define Aladin_ANI_SIT_RIGHT 16			//
#define Aladin_ANI_SIT_LEFT 17			//
#define Aladin_ANI_ONWALL_RIGHT 18			//
#define Aladin_ANI_ONWALL_LEFT 19			//
#define Aladin_ANI_ATTACKED_RIGHT 20			
#define Aladin_ANI_ATTACKED_LEFT 21			

#define Aladin_WIDTH_TMP 30
#define Aladin_HEIGHT_TMP 43
//Tex
#define ID_TEX_Aladin 0
#define ID_TEX_BOX 1
#define ID_TILES_MAP1 10000

// Sprite ID
#define ID_SPRITE_BOXCOLLISION 0

//TileMap

#define MAPTILE_SIZE 16
//Camera
#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 224
#define SPACE_FROM_CAMERA_TO_TOP 20
#define CAMERA_SPEED Aladin_SPEED
#define MAP1_WIDTH 2048
#define MAP1_HEIGHT 176

//Key
#define NON_KEY 0
#define UP_KEY 1
#define DOWN_KEY 2
#define LEFT_KEY 3
#define RIGHT_KEY 4
#define A_KEY 5
#define L_KEY 6
#define D_KEY 7
//Scene
#define GAME_STAGE_1 1
#define GAME_STAGE_2 2

//Grid
#define GRID_CELL_HEIGHT CAMERA_HEIGHT/2
#define GRID_CELL_WIDTH CAMERA_WIDTH/2
#define GRID_CELL_MAX_COLUMN MAP1_WIDTH/GRID_CELL_WIDTH+2 // so dong toi da //+2 tha du con hon thieu
#define GRID_CELL_MAX_ROW MAP1_HEIGHT/GRID_CELL_HEIGHT+2 // so cot toi da

//Type obj
#define TYPE_GROUND 1
//ID = type *100+count;

//Collision
#define OBJ_NO_COLLISION 0
#define OBJ_HAVE_COLLISION 1 //Co va cham ma chua biet huong
#define OBJ_COLLISION_LEFT 2
#define OBJ_COLLISION_TOP 3
#define OBJ_COLLISION_RIGHT 4
#define OBJ_COLLISION_BOTTOM 5

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \

#endif

