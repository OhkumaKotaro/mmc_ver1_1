#ifndef __FUNCTION_H
#define __FUNCTION_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "global.h"

#define IRLED_ON  1
#define IRLED_OFF 0

void setIrledPwm( uint8_t ired );

void adcStart( void );
//void adcEnd( void );

void adcCheckConvert( void );
void update_sensor_data( void );
void getADSensor( int16_t *adcount );
void setSensorConstant( sensor_t *sen, int16_t reference, int16_t threshold );
#ifdef __cplusplus
 }
#endif

#endif /* __FUNCTION_H */