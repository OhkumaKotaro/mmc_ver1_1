#include "mode.h"
#include "global.h"
#include "gpio.h"
#include "tim.h"
#include "motion.h"
#include "control.h"
#include "spi.h"
#include "adc.h"
#include "maze.h"
#include "function.h"

maze_t maze;


/****************************************************************************************
 * outline  : return mode nomber
 * argument : void
 * return   : mode nomber 
********************************************************************************************/
int8_t Mode_select(void){
    int8_t mode=0;
    printf("%d\r",mode);
    while(1){
        if((int16_t)enc.distance_l > 30 || (int16_t)enc.distance_l < -30){
            if((int16_t)enc.distance_l > 30){
                mode++;
            }else if((int16_t)enc.distance_l < -30){
                mode--;
            }
            if(mode > 7){
                mode = 0;
            }else if(mode < 0){
                mode = 7;
            }
            enc.distance_l = 0;
            Output_Buzzer(170-10*mode);
        }
        if(Push()==ON){
            Output_Buzzer(HZ_C_H);
            HAL_Delay(500);
            break;
        }
        LED_Control(mode);
        printf("%d\r",mode);
    }
    return mode;
}


/****************************************************************************************
 * outline  : wright mode 
 * argument : mode nomber
 * return   : void
********************************************************************************************/
void Mode_mouse(int8_t mode){
    switch(mode){
        case 0:
        Sensor_Mode();
        
            //Mode_Adachi();
            /*
            PT_Switch();
            adcStart();
            flag.wall = true;
            while(1){
                if(count_1m==1000){
                    Output_Buzzer(HZ_A);
                    flag.wall = false;
                    break;
                }
            }
            */
            break;
        case 1:
            adcStart();
            flag.wall = ON;
            break;
        case 2:
            PT_Switch();
            gyro_offset_calc_reset();
            HAL_Delay(2000);
            Motion_Left();
            break;
        case 3:
            PT_Switch();
            gyro_offset_calc_reset();
            HAL_Delay(2000);
            Motion_Right();
            break;
        case 4:
            PT_Switch();
            gyro_offset_calc_reset();
            Motion_Uturn();
            break;
        case 5:
            PT_Switch();
            setIrledPwm(ON);
            gyro_offset_calc_reset();
            HAL_Delay(2000);
            Motion_Straight_Check();
            break;
        case 6:
            //Show_log();
            
            break;
        case 7:
            Show_log();
            break;
    }
    HAL_Delay(1000);
}



void Mode_Adachi(void){
    uint8_t flag_goal_is=false;
    int8_t next_dir;

   
    Init_maze();
    PT_Switch();
    gyro_offset_calc_reset();
    HAL_Delay(2000);
    setIrledPwm(ON);
    Motion_Start();
    position.x=0;
    position.y=1;
    position.dir=NORTH;
    Maze_Set();

    adcStart();
    
    while(flag_goal_is==false){
        Maze_Get_Wall(position.x,position.y);
        MAZE_Create_Step();
        HAL_Delay(200);
        next_dir=Maze_Next_Motion();
        Update_Position(next_dir);
        if(position.x==mazeDef.maze_goal_x && position.y==mazeDef.maze_goal_y){
            flag_goal_is=true;
        }

        switch( next_dir ){
            case LEFT:
                Motion_Left();
                break;

            case STRAIGHT:
                Motion_Straight();
                break;
            
            case RIGHT:
                Motion_Right();
                break;

            case UTURN:
                Motion_Uturn();
                break;
            case KABEATE:
                Motion_Kabeate();
                break;
        }
    }
    Motion_Goal();
}

void Mode_Circuit(void){
    PT_Switch();
    gyro_offset_calc_reset();
    HAL_Delay(2000);
    Motion_Straight_Check();
    //Motion_Start();    
}