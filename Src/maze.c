#include "maze.h"
#include "gpio.h"
#include "global.h"


/**************variable**********************/
mazeDef_t mazeDef;
maze_t maze;



/****************************************************************************************************
 * outline : input maze size , goal point
 * argument : void
 * return : void
******************************************************************************************************/
void Maze_Set(void){
    mazeDef.maze_goal_x = 7;
    mazeDef.maze_goal_y = 7;
}

/****************************************************************************************************
 * outline : initialize maze date
 * argument : void
 * return : void
******************************************************************************************************/
void Init_maze(void) {
	//init wall and is_search
	for (int i = 0; i <= MAZE_SIZE; i++)
	{
		for (int j = 0; j <= MAZE_SIZE; j++)
		{
			maze.wall_east[i][j] = 0;
			maze.wall_north[i][j] = 0;
			maze.wall_south[i][j] = 0;
			maze.wall_west[i][j] = 0;
			maze.is_search[i][j] = 0;
			maze.goal[i][j] = 0;
		}
	}
	//input start wall
	maze.wall_east[0][0] = 1;
	maze.wall_west[1][0] = 1;
	//input outer
	for (int i = 0; i <= MAZE_SIZE; i++)
	{
		maze.wall_east[MAZE_SIZE][i] = 1;
		maze.wall_west[0][i] = 1;
        maze.wall_south[i][0] = 1;
		maze.wall_north[i][MAZE_SIZE] = 1;
	}
}


/****************************************************************************************************
 * outline : create map
 * argument : void
 * return : void
******************************************************************************************************/
void MAZE_Create_Step( void ){
	/* <概要>   : 歩数マップ作成
	 * <引数>   : 走行の状態( 探索,最短 )
	 * <戻り値> : なし
	 */
	// 初期化
	for( int i = 0; i <= MAZE_SIZE; i++ ){
		for( int j = 0; j <= MAZE_SIZE; j++ ){
			if( i == mazeDef.maze_goal_x && j == mazeDef.maze_goal_y ){
				maze.step[i][j] = 0;
			}
			else{
				maze.step[i][j] = MAX_STEP;
			}
		}
	}

	volatile uint8_t end_flag = true;	// 終了フラグ
	uint8_t stepNum = 0;		// 設定中の歩数
	// 探索
	
	while( end_flag == true ){
		end_flag = false;
		uint8_t i=0;
		for( i = 0; i <= MAZE_SIZE; i++ ){
			uint8_t j=0;
			for( j = 0; j <= MAZE_SIZE; j++ ){
				if( maze.step[i][j] == MAX_STEP ){
					if( j < MAZE_SIZE){	//北壁
						if( (maze.wall_north[i][j] == false)&&(maze.step[i][j+1] == stepNum) ){
							maze.step[i][j] = stepNum + 1;
							end_flag = true;
						}
					}
					if( i < MAZE_SIZE){	//東壁
						if( (maze.wall_east[i][j] == false)&&(maze.step[i+1][j] == stepNum) ){
							maze.step[i][j] = stepNum + 1;
							end_flag = true;
						}
					}
					if( j > 0 ) {					//南壁
						if( (maze.wall_south[i][j] == false)&&(maze.step[i][j-1] == stepNum) ){
							maze.step[i][j] = stepNum + 1;
							end_flag = true;
						}
					}
					if( i > 0 ){					//西壁
						if( (maze.wall_west[i][j] == false)&&(maze.step[i-1][j] == stepNum) ){
							maze.step[i][j] = stepNum + 1;
							end_flag = true;
						}
					}
				}
			}
		}
		if (end_flag == true) {
			stepNum++;
		}
	}
	
}

void Maze_Get_Wall( int8_t x, int8_t y){
	int n_wall=0, e_wall=0, s_wall=0, w_wall=0;

	if(sen_front.is_wall==false){
    	flag.kabeate=false;
    }else{
        flag.kabeate=true;
    }
	// 探索済みに設定
	maze.is_search[x][y] = true;
	// 方向別に壁の状態を取得
	switch( position.dir ){
	case NORTH:
		n_wall = sen_front.is_wall;
		e_wall = sen_r.is_wall;
		w_wall = sen_l.is_wall;
		s_wall = 0;
		break;
	case EAST:
		e_wall = sen_front.is_wall;
		s_wall = sen_r.is_wall;
		n_wall = sen_l.is_wall;
		w_wall = 0;
		break;
	case SOUTH:
		s_wall = sen_front.is_wall;
		w_wall = sen_r.is_wall;
		e_wall = sen_l.is_wall;
		n_wall = 0;
		break;
	case WEST:
		w_wall = sen_front.is_wall;
		n_wall = sen_r.is_wall;
		s_wall = sen_l.is_wall;
		e_wall = 0;
		break;
	default:
		break;
	}
	// 方向別に保存
	maze.wall_north[x][y] = n_wall;
	maze.wall_east[x][y] = e_wall;
	maze.wall_south[x][y] = s_wall;
	maze.wall_west[x][y] = w_wall;
	// 隣り合う壁の情報を保存
	if( y < MAZE_SIZE-1 ){
		maze.wall_south[x][y+1] = n_wall;
	}
	if( x < MAZE_SIZE-1){
		maze.wall_west[x+1][y] = e_wall;
	}
	if(y > 0){
		maze.wall_north[x][y-1] = s_wall;
	}
	if(x > 0){
		maze.wall_east[x-1][y] = w_wall;
	}
}

void Update_Position(int8_t next_motion){
	switch(next_motion){
		case LEFT:
			switch(position.dir){
				case NORTH:
					position.x --;
					position.dir = WEST;
					break;
				case EAST:
					position.y ++;
					position.dir = NORTH;
					break;
				case SOUTH:
					position.x ++;
					position.dir = EAST;
					break;
				case WEST:
					position.y --;
					position.dir = SOUTH;
					break;
			}
			break;
		case STRAIGHT:
			switch(position.dir){
				case NORTH:
					position.y ++;
					break;
				case EAST:
					position.x ++;
					break;
				case SOUTH:
					position.y --;
					break;
				case WEST:
					position.x --;
					break;
			}
			break;
		case RIGHT:
			switch(position.dir){
				case NORTH:
					position.x ++;
					position.dir = EAST;
					break;
				case EAST:
					position.y --;
					position.dir = SOUTH;
					break;
				case SOUTH:
					position.x --;
					position.dir = WEST;
					break;
				case WEST:
					position.y ++;
					position.dir = NORTH;
					break;
			}
			break;
		case UTURN:
			switch(position.dir){
				case NORTH:
					position.y --;
					position.dir = SOUTH;
					break;
				case EAST:
					position.x --;
					position.dir = WEST;
					break;
				case SOUTH:
					position.y ++;
					position.dir = NORTH;
					break;
				case WEST:
					position.x ++;
					position.dir = EAST;
					break;
			}
			break;
		case KABEATE:
			switch(position.dir){
				case NORTH:
					position.y --;
					position.dir = SOUTH;
					break;
				case EAST:
					position.x --;
					position.dir = WEST;
					break;
				case SOUTH:
					position.y ++;
					position.dir = NORTH;
					break;
				case WEST:
					position.x ++;
					position.dir = EAST;
					break;
			}
			break;
	}
}

int8_t Maze_Next_Motion(void){
	/* <概要>   : 次の動作を決定
	 * <引数>   : なし
	 * <戻り値> : 次の動作
	 */
	uint8_t tmp_step = MAX_STEP;		// 歩数
	int8_t tmp_dir = UTURN;				// 方向
	// 現在の向きに応じて場合分けし、 歩数が少ない方向を判断
	// 迷路外に進むのとゴールがスタートマス以外の場合(0,0)に進むのを阻止
	switch( position.dir ){
		case NORTH:
			if( maze.step[position.x][position.y+1] < tmp_step ){
				if( position.y < MAZE_SIZE ){
					if( maze.wall_north[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y+1];
						tmp_dir = STRAIGHT;
					}
				}
			}
			if( maze.step[position.x-1][position.y] < tmp_step ){
				if( position.x > 0 ){
					if( maze.wall_west[position.x][position.y] == false ){
						tmp_step = maze.step[position.x-1][position.y];
						tmp_dir = LEFT;
					}
				}
			}
			if( maze.step[position.x+1][position.y] < tmp_step ){
				if( position.x < MAZE_SIZE ){
					if( maze.wall_east[position.x][position.y] == false ){
						tmp_step = maze.step[position.x+1][position.y];
						tmp_dir = RIGHT;
					}
				}
			}
			if( tmp_step == MAX_STEP){
				if(flag.kabeate == false){
					tmp_dir = UTURN;
				}else if(flag.kabeate == true){
					tmp_dir = KABEATE;
				}
				
			}
			break;
		case EAST:
			if( maze.step[position.x+1][position.y] < tmp_step ){
				if( position.x < MAZE_SIZE ){
					if( maze.wall_east[position.x][position.y] == false ){
						tmp_step = maze.step[position.x+1][position.y];
						tmp_dir = STRAIGHT;
					}
				}
			}
			if( maze.step[position.x][position.y+1] < tmp_step ){
				if( position.y < MAZE_SIZE ){
					if( maze.wall_north[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y+1];
						tmp_dir = LEFT;
					}
				}
			}
			if( maze.step[position.x][position.y-1] < tmp_step ){
				if( position.y > 0 ){
					if( maze.wall_south[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y-1];
						tmp_dir = RIGHT;
					}
				}
			}
			if( tmp_step == MAX_STEP){
				if(flag.kabeate == false){
					tmp_dir = UTURN;
				}else if(flag.kabeate == true){
					tmp_dir = KABEATE;
				}
			}
			break;
		case SOUTH:
			if( maze.step[position.x][position.y-1] < tmp_step ){
				if( position.y > 0 ){
					if( maze.wall_south[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y-1];
						tmp_dir = STRAIGHT;
				}
				}
			}
			if( maze.step[position.x+1][position.y] < tmp_step ){
				if( position.x < MAZE_SIZE){
					if( maze.wall_east[position.x][position.y] == false ){
						tmp_step = maze.step[position.x+1][position.y];
						tmp_dir = LEFT;
					}
				}
			}
			if( maze.step[position.x-1][position.y] < tmp_step ){
				if( position.x > 0 ){
					if( maze.wall_west[position.x][position.y] == false ){
						tmp_step = maze.step[position.x-1][position.y];
						tmp_dir = RIGHT;
					}
				}
			}
			if( tmp_step == MAX_STEP){
				if(flag.kabeate == false){
					tmp_dir = UTURN;
				}else if(flag.kabeate == true){
					tmp_dir = KABEATE;
				}
			}
			break;
		case WEST:
			if( maze.step[position.x-1][position.y] < tmp_step ){
				if( position.x > 0 ){
					if( maze.wall_west[position.x][position.y] == false ){
						tmp_step = maze.step[position.x-1][position.y];
						tmp_dir = STRAIGHT;
					}
				}
			}
			if( maze.step[position.x][position.y-1] < tmp_step ){
				if( position.y > 0 ){
					if( maze.wall_south[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y-1];
						tmp_dir = LEFT;
					}
				}
			}
			if( maze.step[position.x][position.y+1] < tmp_step ){
				if( position.y < MAZE_SIZE ){
					if( maze.wall_north[position.x][position.y] == false ){
						tmp_step = maze.step[position.x][position.y+1];
						tmp_dir = RIGHT;
					}
				}
			}
			if( tmp_step == MAX_STEP){
				if(flag.kabeate == false){
					tmp_dir = UTURN;
				}else if(flag.kabeate == true){
					tmp_dir = KABEATE;
				}
			}
			break;
		default:
			break;
	}
	return tmp_dir;
}

/****************************************************************************************************
 * outline : output step
 * argument : void
 * return : void
******************************************************************************************************/
void MAZE_Printf_Step(void) {
	int i=0;
	for (i = 0; i <= MAZE_SIZE; i++)
	{
		int j;
		for (j = 0; j <= MAZE_SIZE; j++)
		{
			printf("+");
			if (maze.wall_west[i][j] == 1)
			{
				printf("---");
			}
			else if(maze.wall_west[i][j] == 0)
			{
				printf("   ");
			}
		}
		printf("+\r\n");
		for (j = 0; j <= MAZE_SIZE; j++)
		{
			if (maze.wall_south[i][j] == 1)
			{
				printf("|");
			}
			else
			{
				printf(" ");
			}
			printf("%3d", maze.step[i][j]);
		}
		printf("|\r\n");
	}
	for (i = 0; i <= MAZE_SIZE; i++)
	{
		printf("+");
		printf("---");
	}
	printf("+\r\n\n");
}

void Maze_GoalCheck(uint8_t flag_goal_is){
	if(position.x==mazeDef.maze_goal_x && position.y==mazeDef.maze_goal_y){
		flag_goal_is = true;
	}
}