#ifndef __GLOBAL_H
#define __GLOBAL_H
#ifdef __cplusplus
 extern "C" {
#endif

/***Includes***********************************************************************/
#include "stdint.h"
#include <stdio.h>
#include <string.h>

/***define************************************************************************/

//0,1
#define true  1
#define false 0

#define ON 1
#define OFF 0

#define LEFT 1
#define STRAIGHT 0
#define RIGHT -1
#define UTURN 2
#define KABEATE 3

//motor 003SR
#define KT  1.98f//toluku[mNm/A]
#define KE  0.207f//return vatt[mV/min^-1]
#define Resistance 1.07f//[S^-1]
#define TORQUE 0.2


//encoder
#define ENC_CUL_ROT 1024.0f//512 * 2


//mouse
#define WEIGHT 100.0f //[g]
#define TIRE_RADIUS 12.3f  //[mm] 直径24.58mm
#define INERTIA     23.0f 
#define TREAD 73.0f  //[mm]
#define GEAR_RATE 5.25f

//calc
#define dt 0.001f

//straight
#define ACCEL 4000 //[mm/s^2]
#define MAX_VELOCITY 300 //[mm/s]

//yawrate
#define Y_ACCEL 2000 //[degree/s^2]//2000
#define Y_MAX_VELOCITY 380//[degree/s]

//adc
#define ADC_CONVERT_DATA_SIZE 8


/***typedef**************************************************************************/

//flag
typedef struct{
  uint8_t gyro_calc;
  uint8_t straight;
  uint8_t yawrate;
  uint8_t straight_zero;
  uint8_t yawrate_zero;
  uint8_t accel;
  int16_t dir;
  uint8_t motion_end;
  uint8_t wall;
  int8_t next_dir;
  uint8_t kabeate;
}flag_t;
extern volatile flag_t flag;


//gyro
typedef struct{
  short offset_cnt;
  int32_t offset;
  float befor;
  float degree;
  float velocity;
  float accel;
}gyro_t;
extern gyro_t gyro;


//encoder
typedef struct{
  int left;
  int right;
  float rpm_l;
  float rpm_r;
  float velocity_l;
  float velocity_r;
  float velocity_c;
  float distance_l;
  float distance_r;
  float old_l;
  float old_r;
}enc_t;
extern enc_t enc;


typedef struct{
  float target_velocity[2000];
  float target_y_velocity[2000];
  float velocity_c[2000];
  float y_velocity[2000];
  int16_t pwm[2000];
  int16_t sen_l[2000];
  int16_t sen_r[2000];
}loger_t;
extern loger_t loger;

//batt
extern uint16_t batt_analog;
extern float batt_Vcc;

//motor parameters
typedef struct{
  int16_t pwm_l;
  int16_t pwm_r;
  int16_t accel_l;
  int16_t accel_r;
}motor_t;
extern motor_t motor;

//calc
typedef struct{
  float distance;
  int16_t velocity;
  int16_t accel;

  float yawrate_degree;
  int16_t yawrate_velocity;
  int16_t yawrate_accel;
}calc_t;
extern calc_t calc;

extern float s_accel;


//mouse position
typedef struct{
  uint8_t x;
  uint8_t y;
  uint8_t dir;
}position_t;
extern position_t position;


//count
extern uint16_t count_1m;
extern uint16_t straight_cnt;
extern uint16_t yawrate_cnt;

//straight
extern uint16_t accel_T;
extern uint16_t constant_T;
extern uint16_t decrease_T;

//yawrate
extern uint16_t y_accel_T;
extern uint16_t y_constant_T;
extern uint16_t y_decrease_T;


// sensor data
typedef struct {
  int16_t now;
  int16_t befor_1ms;
  int16_t reference;      // 真ん中のときのセンサー値
  int16_t threshold;      // 閾値
  int16_t diff;     // 差分
  int16_t diff_1ms; // 1msec前
  uint8_t is_wall;        // 壁があるかどうか判断
  // 斜め閾値
}sensor_t;

// sensor data 
extern sensor_t sen_front; // 前壁
extern sensor_t sen_fl;
extern sensor_t sen_fr;
extern sensor_t sen_l; // 横左壁
extern sensor_t sen_r; // 横右壁

extern uint16_t ADCBuff[ADC_CONVERT_DATA_SIZE];
extern uint16_t ADCOffData[ADC_CONVERT_DATA_SIZE];
extern uint16_t ADCOntData[ADC_CONVERT_DATA_SIZE];
extern int16_t adc_counter;


#ifdef __cplusplus
}
#endif
#endif