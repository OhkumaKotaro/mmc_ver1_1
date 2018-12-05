#ifndef __CONTROL_H
#define __CONTROL_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"


#define TARGET_WALL 0.0f
#define WALL_LEFT_ON 2414
#define WALL_RIGHT_ON 2530 
#define WALL_LEFT_OFF 2030
#define WALL_RIGHT_OFF 2300

float s_sum_l,s_sum_r;
float y_sum,y_sumsum;
float w_sum;
float w_befor;
int16_t straight_pid_l,straight_pid_r;
int16_t yawrate_pid;
int16_t wall_pid;
int8_t straight_dir;


float PID_value(float target,float measured,float *sum,float *old,float Kp,float Ki,float Kd);
float Integral_value(float error,float *sum,float Kii);
void Calc_Palam(int16_t accel,int16_t *velocity,uint16_t *calc_tim);
void Straight_Calc_fb(int16_t distant,int16_t v_start,int16_t v_end);
void Straight_Calc_Zero(void);
void Straight_SysTic_fb(void);
void Yawrate_Calc_fb(int16_t degree,int16_t v_start,int16_t v_end);
void Yawrate_Calc_Zero(void);
void Yawrate_SysTic_fb(void);
void Control_Wall(void);
void Control_pwm(void);
void Output_Buzzer(uint8_t Hz);
void control_accel(int16_t accel_l,int16_t accel_r );
void straight_one_ff(void);
void Sensor_Check(void);


#ifdef __cplusplus
}
#endif
#endif