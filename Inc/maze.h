#ifndef __MAZE_H
#define __MAZE_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#define MAX_STEP 254
#define MAZE_SIZE 15
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

typedef struct {							
	uint8_t maze_goal_x;							// ゴールマス( X )
	uint8_t maze_goal_y;							// ゴールマス( Y )
}mazeDef_t;

typedef struct {								// 迷路情報
	uint8_t step[MAZE_SIZE+1][MAZE_SIZE+1];							// 歩数マップ
	uint8_t goal[MAZE_SIZE+1][MAZE_SIZE+1];
	uint8_t wall_north[MAZE_SIZE+1][MAZE_SIZE+1];						// 壁情報
	uint8_t wall_east[MAZE_SIZE+1][MAZE_SIZE+1];						// 壁情報
	uint8_t wall_south[MAZE_SIZE+1][MAZE_SIZE+1];						// 壁情報
	uint8_t wall_west[MAZE_SIZE+1][MAZE_SIZE+1];						// 壁情報
	uint8_t is_search[MAZE_SIZE+1][MAZE_SIZE+1];						// 探索済みフラグ
	uint8_t step_mode;						// 歩数マップ作成時の状態(探索用 最短走行用)
}maze_t;

extern maze_t maze;


extern mazeDef_t mazeDef;


void Init_maze(void);
void Maze_Set(void);
void MAZE_Create_Step(void);
void Maze_Get_Wall( int8_t x, int8_t y);
void Update_Position(int8_t next_motion);
int8_t Maze_Next_Motion(void);
void MAZE_Printf_Step(void);
void Maze_GoalCheck(uint8_t flag_goal_is);


#ifdef __cplusplus
}
#endif
#endif