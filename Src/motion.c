#include "motion.h"
#include "tim.h"
#include "global.h"
#include "control.h"
#include "gpio.h"
#include "spi.h"
#include "maze.h"
#include "function.h"

static unsigned char flag_kabeate;

void Normal_Turn_Half(void){
    PT_Switch();
    gyro_offset_calc_reset();
    Yawrate_Calc_fb(180,0,0);
    HAL_Delay(1500);
    Output_Buzzer(HZ_C_H);
    flag.yawrate = ON;
    while(1){
        if(Push()==ON){
            flag.yawrate = OFF;
            Output_Buzzer(HZ_A);
            break;
        }
    }
}

/****************************************************************************************
 * outline  : "ennkaigei"
 * argument : void
 * return   : void
********************************************************************************************/
void Normal_Ennkai(void){
    PT_Switch();
    HAL_Delay(500);
    gyro_offset_calc_reset();
    Yawrate_Calc_Zero();
    HAL_Delay(1500);
    Output_Buzzer(HZ_C_H);
    flag.yawrate = ON;
    while(1){
        if(Push()==ON){
            flag.yawrate = OFF;
            Output_Buzzer(HZ_A);
            break;
        }
    }
}

void Ennkai(void){
    PT_Switch();
    HAL_Delay(500);
    gyro_offset_calc_reset();
    Yawrate_Calc_Zero();
    Straight_Calc_Zero();
    HAL_Delay(1500);
    Output_Buzzer(HZ_C_H);
    flag.yawrate = ON;
    flag.straight = ON;
    while(1){
        if(Push()==ON){
            flag.yawrate = OFF;
            flag.straight = OFF;
            Output_Buzzer(HZ_A);
            break;
        }
    }
}


/****************************************************************************************
 * outline  : straight run one block by feadbuck control
 * argument : void
 * return   : void
********************************************************************************************/
void Normal_Straight(void){
    PT_Switch();
    HAL_Delay(500);
    Straight_Calc_fb(180,0,0);
    HAL_Delay(1000);
    Output_Buzzer(HZ_C_H);
    flag.straight = ON;
    while(1){
        if(Push()==ON){
            flag.straight = OFF;
            Output_Buzzer(HZ_A);
            break;
        }
    }
}


/****************************************************************************************
 * outline  : straight one block by encoder and gyro
 * argument : void
 * return   : void
********************************************************************************************/
void Straight_Start(void){
    Straight_Calc_fb(136.4,0,0);
    Yawrate_Calc_Zero();
    flag.straight = ON;
    flag.yawrate = ON;
    flag.wall = OFF;
}

void Straight(void){
    Straight_Calc_fb(180,0,0);
    Yawrate_Calc_Zero();
    flag.straight = ON;
    flag.yawrate = ON;
    flag.wall = ON;
}

void Straight_HalF(void){
    Straight_Calc_fb(90,0,0);
    Yawrate_Calc_Zero();
    flag.straight = ON;
    flag.yawrate = ON;
    flag.wall = ON;
}

void Turn_Half(void){
    flag.dir = LEFT;
    Yawrate_Calc_fb(180,0,0);
    Straight_Calc_Zero();
    flag.wall = OFF;
    flag.yawrate = ON;
    flag.straight = ON;
}

void Turn_Quarter_Left(void){
    flag.dir = LEFT;
    Yawrate_Calc_fb(90,0,0);
    Straight_Calc_Zero();
    flag.wall = OFF;
    flag.yawrate = ON;
    flag.straight = ON;
}

void Turn_Quarter_Right(void){
    flag.dir = RIGHT;
    Yawrate_Calc_fb(90,0,0);
    Straight_Calc_Zero();
    flag.wall = OFF;
    flag.yawrate = ON;
    flag.straight = ON;
}

void Back(void){
    Straight_Calc_fb(-55,0,0);
    Yawrate_Calc_Zero();
    flag.wall = OFF;
    flag.straight = ON;
    flag.yawrate = ON;
}

void Straight_Check(void){
    Straight_Calc_fb(1080,0,0);
    Yawrate_Calc_Zero();
    HAL_Delay(10);
    Output_Buzzer(HZ_C_H);
    flag.straight = ON;
    flag.wall=OFF;
    flag.yawrate = ON;
}

void Motion_Start(void){
    Back();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Straight_Start();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Straight(void){
    Straight();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Left(void){
    Straight_HalF();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Turn_Quarter_Left();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Straight_HalF();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Right(void){
    Straight_HalF();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Turn_Quarter_Right();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Straight_HalF();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Uturn(void){
    Straight_HalF();
    while( flag.accel==ON || flag.straight==ON ){}
    HAL_Delay(500);
    Turn_Half();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Straight_HalF();
    while(flag.accel==ON || flag.straight==ON){}
    
    Output_Buzzer(HZ_C_H);
}

void Motion_Kabeate(void){
    Straight_HalF();
    while( flag.accel==ON || flag.straight==ON ){}
    HAL_Delay(500);
    Turn_Half();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Back();
    while(flag.accel==ON || flag.straight==ON){}
    HAL_Delay(500);
    Straight_Start();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Goal(void){
    Straight_Start();
    while( flag.accel==ON || flag.straight==ON ){}
    Output_Buzzer(HZ_C_H);
}

void Motion_Straight_Check(void){
    Straight_Check();
    while( flag.accel==ON || flag.straight==ON ){}
}

void Motion_Back(void){
    Back();
    while(flag.accel==ON || flag.straight==ON){}
    Output_Buzzer(HZ_C_H);
}

void Motion_nStraight(unsigned char num){
    for(uint8_t i=0;i<num;i++){
        Straight();
        while(flag.accel==ON || flag.straight==ON){}
    }
}
/****************************************************************************************
 * outline  : show log
 * argument : void
 * return   : void
********************************************************************************************/
void Show_log(void){
    printf("\na:%d,c:%d,d:%d\r\n",y_accel_T,y_constant_T,y_decrease_T);
    for(uint16_t i=0;i<2000;i++){
        printf("%d\t%d\r\n",loger.sen_l[i],loger.sen_r[i]);
        if(Push()==ON){
            Output_Buzzer(HZ_A);
            HAL_Delay(300);
            break;
        }
    }
}

void Sensor_Mode(void){
    while(1){
        adcStart();
        Sensor_Check();
        //printf("left:%d front:%d right:%d\r",sen_l.now,sen_front.now,sen_r.now);
        Control_Wall();
        if(Push()==ON){
            Output_Buzzer(HZ_A);
            break;
        }
    }
}

void Test_Create_Map(void){
    Maze_Set();
    Init_maze();
    MAZE_Create_Step();
    MAZE_Printf_Step();
}

/****************************************************************************************
 * outline  : normalize battery (12bit -> dec)
 * argument : void
 * return   : void
********************************************************************************************/
void Batt_Check(void)
{
  float batt=0;
  for(int i=0;i<10;i++){
    batt += batt_analog;
  }
  batt /= 10.0f;
  batt = batt/4095.0f*133.0f/33.0f*3.3f;
  batt_Vcc = batt;
  printf("\nbatt:%lf\r\n",batt);
}

void PT_Switch(void){
    adcStart();
    while(1){
        if(sen_front.is_wall==true){
        Output_Buzzer(HZ_C_H);
        break;
        }
    }
}

void Kabeate_dec(unsigned char flag){
    flag_kabeate = flag;
}