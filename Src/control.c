#include "control.h"
#include <stdint.h>
#include "global.h"
#include "tim.h"
#include "arm_math.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "function.h"

float encoder_Kp = 0;
float encoder_Ki = 0;
float encoder_Kd = 0;

/****************************************************************************************
 * outline  : PID control
 * argument : 
 * return   : control value
********************************************************************************************/
float PID_value(float target,float measured,float *sum,float *old,float Kp,float Ki,float Kd){
    float error;
    float p,i,d;

    error = target - measured;
    p = Kp*error;

    *sum += error * dt;
    i = *sum * Ki;

    d = Kd * (error - *old);
    *old = error;

    return (p+i+d);
}

float Integral_value(float error,float *sum,float Kii){
    float ii;
    *sum += error * dt;
    ii = *sum * Kii;
    return ii;
}

/****************************************************************************************
 * outline  : calculate parameter
 * argument : accel,*velocity,*distance
 * return   : void
********************************************************************************************/
void Calc_Palam(int16_t accel,int16_t *velocity,uint16_t *calc_tim){
    *calc_tim += 1;
    *velocity += accel * dt;
}


/****************************************************************************************
 * outline  : calcurate accele distance
 * argument : distance[mm],v_start[mm/s],v_end[mm/s]
 * return   : void
********************************************************************************************/
void Straight_Calc_fb(int16_t distant,int16_t v_start,int16_t v_end){
    float t1,t2,t3;
    float constant_L;

    calc.velocity = v_start;
    enc.old_l = 0;
    enc.old_r = 0;
    s_sum_l = 0;
    s_sum_l = 0;
    flag.straight_zero = OFF;

    if(distant<0){
        distant = -1*distant;
        straight_dir=-1;
    }else{
        straight_dir=1;
    }

    t1 = (float)(MAX_VELOCITY - v_start) / ACCEL;
    t3 = (float)(MAX_VELOCITY - v_end) / ACCEL;

    constant_L = (float)distant - (MAX_VELOCITY + v_start) * t1 / 2 - (MAX_VELOCITY + v_start) * t3 / 2;

    t2 = constant_L / MAX_VELOCITY;

    t1 *= 1000;
    t2 *= 1000;
    t3 *= 1000;

    accel_T = t1;
    constant_T = t2;
    decrease_T = t3;

    encoder_Kp = 4.0f;
    encoder_Ki = 10.0f;
    encoder_Kd = 0;
}

void Straight_Calc_Zero(void){
    flag.straight_zero=ON;
    encoder_Kp = 4.0;  //4.0
    encoder_Ki = 10.0;
    encoder_Kd = 0;
}

/****************************************************************************************
 * outline  : output pwm for trapezoid accele straight by feadbuck control
 * argument : void
 * return   : void
********************************************************************************************/
void Straight_SysTic_fb(void){
    float encoder_l,encoder_r;
    float target = 1;
    if(flag.straight_zero == ON){
        encoder_l = enc.velocity_c;
        encoder_r = enc.velocity_c;
        calc.velocity = 0;
    }
    else if(straight_cnt < accel_T){
        encoder_l = enc.velocity_l;
        encoder_r = enc.velocity_r;
        Calc_Palam(ACCEL,&calc.velocity,&straight_cnt);
    }
    else if(straight_cnt < constant_T + accel_T){
        encoder_l = enc.velocity_l;
        encoder_r = enc.velocity_r;
        Calc_Palam(0,&calc.velocity,&straight_cnt);
    }
    else if(straight_cnt < decrease_T + constant_T + accel_T){
        encoder_l = enc.velocity_l;
        encoder_r = enc.velocity_r;
        Calc_Palam(-ACCEL,&calc.velocity,&straight_cnt);
    }else{
        encoder_l = enc.velocity_l;
        encoder_r = enc.velocity_r;
        calc.velocity = 0;
        if(enc.velocity_c==0){
            flag.straight = OFF;
            flag.yawrate = OFF;
        }
        //flag.wall = OFF;
    }

    target = straight_dir * calc.velocity;
    
    straight_pid_l = (int16_t)PID_value(target,encoder_l,&s_sum_l,&enc.old_l,encoder_Kp,encoder_Ki,encoder_Kd);
    straight_pid_r = (int16_t)PID_value(target,encoder_r,&s_sum_r,&enc.old_r,encoder_Kp,encoder_Ki,encoder_Kd);
}


/****************************************************************************************
 * outline  : calcurate trapezoid accel
 * argument : degree[degree],v_start[degree/s],v_end[degree/s]
 * return   : void
********************************************************************************************/
void Yawrate_Calc_fb(int16_t degree,int16_t v_start,int16_t v_end){
    calc.yawrate_velocity = v_start;
    y_sum = 0;
    gyro.befor = 0;
    flag.yawrate_zero = OFF;

    float t1=0,t2=0,t3=0;
    float constant_L;

    t1 = (float)(Y_MAX_VELOCITY - v_start) / Y_ACCEL;
    t3 = (float)(Y_MAX_VELOCITY - v_end) / Y_ACCEL;

    constant_L = (float)degree - (Y_MAX_VELOCITY + v_start) * t1 / 2 - (Y_MAX_VELOCITY + v_end) * t3 / 2;
    
    t2 = constant_L / Y_MAX_VELOCITY;

    t1 *= 1000;
    t2 *= 1000;
    t3 *= 1000;

    y_accel_T = t1;
    y_constant_T = t2;
    y_decrease_T = t3;
}

void Yawrate_Calc_Zero(void){
    flag.yawrate_zero=ON;
}

/****************************************************************************************
 * outline  : call 1ms (control yawrate by feadbuck)
 * argument : void
 * return   : void
********************************************************************************************/
void Yawrate_SysTic_fb(void){
    if(flag.yawrate_zero==ON){
        calc.yawrate_velocity = 0;
    }
    else if(yawrate_cnt < y_accel_T){
        Calc_Palam(Y_ACCEL,&calc.yawrate_velocity,&yawrate_cnt);
    }else if(yawrate_cnt < y_constant_T + y_accel_T){
        Calc_Palam(0,&calc.yawrate_velocity,&yawrate_cnt);
    }else if(yawrate_cnt < y_decrease_T + y_constant_T + y_accel_T){
        Calc_Palam(-Y_ACCEL,&calc.yawrate_velocity,&yawrate_cnt);
    }else{
        calc.yawrate_velocity = 0;
        //if(gyro.velocity==0){
            flag.yawrate = OFF;
            flag.straight = OFF;
        //}
    }
    yawrate_pid = (int16_t)PID_value((float)flag.dir * calc.yawrate_velocity,gyro.velocity,&y_sum,&gyro.befor,0.92f,9.0f,4.0f);
}

void Control_Wall(void){
    int16_t wall_dif = 0;

    wall_dif = (sen_l.now - 573 ) - (sen_r.now - 670 );
    wall_pid = (int16_t)(0.1f * (float)wall_dif);

    printf("%d\t%d\r\n",wall_pid,sen_r.now);
}

void Control_pwm(void){
    int16_t pwm_l=0,pwm_r=0;
    if(flag.wall==ON){
        pwm_l += wall_pid;
        pwm_r -= wall_pid;
    }

    if(flag.straight==ON){
        pwm_l += straight_pid_l;
        pwm_r += straight_pid_r;
    }
    if(flag.yawrate==ON){
        pwm_l -= yawrate_pid;
        pwm_r += yawrate_pid;
    }
    Motor_pwm(pwm_l,pwm_r);
}
/****************************************************************************************
 * outline  : turn on buzzer 0.3sec
 * argument : Hz
 * return   : void
********************************************************************************************/
void Output_Buzzer(uint8_t Hz){
    Buzzer_pwm(Hz,300);
    HAL_Delay(100);
    Buzzer_pwm(HZ_NORMAL,0);
}


/****************************************************************************************
 * outline  : sensor check
 * argument : void
 * return   : void
********************************************************************************************/
void Sensor_Check(void){
    if(sen_r.is_wall==true){
        HAL_GPIO_WritePin(led0_GPIO_Port,led0_Pin,GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(led0_GPIO_Port,led0_Pin,GPIO_PIN_SET);
    }
    if(sen_l.is_wall==true){
        HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(led1_GPIO_Port,led1_Pin,GPIO_PIN_SET);
    }
    if(sen_front.is_wall==true){
        HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(led3_GPIO_Port,led3_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(led2_GPIO_Port,led2_Pin,GPIO_PIN_SET);
    } 
}
