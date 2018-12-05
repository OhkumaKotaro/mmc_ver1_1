#include "global.h"

volatile flag_t flag;

gyro_t gyro;
enc_t enc;
calc_t calc;
float s_accel;
position_t position;

uint16_t count_1m;
uint16_t batt_analog;
float batt_Vcc;



uint16_t straight_cnt;
uint16_t yawrate_cnt;

motor_t motor;

loger_t loger;

uint16_t accel_T;
uint16_t constant_T;
uint16_t decrease_T;

uint16_t y_accel_T;
uint16_t y_constant_T;
uint16_t y_decrease_T;


// sensor data 
sensor_t sen_front; // 前壁
sensor_t sen_fl;
sensor_t sen_fr;
sensor_t sen_l; // 横左壁
sensor_t sen_r; // 横右壁

uint16_t ADCBuff[ADC_CONVERT_DATA_SIZE];
uint16_t ADCOffData[ADC_CONVERT_DATA_SIZE];
uint16_t ADCOntData[ADC_CONVERT_DATA_SIZE];
int16_t adc_counter;