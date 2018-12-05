#include "function.h"
// peripheeral
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "spi.h"
#include "main.h"
#include "gpio.h"

//common

#define OFF_VALUE 0
#define LEFT_VALUE 1
#define RIGHT_VALUE 2
#define FINISH_CONVERT 3

#define true 1
#define false 0

static uint8_t ctr_irled = 0;

///////////////////////////////////////////////////////////////////////
// able ir led
// [argument] ired 1:paluse emit ON 0:OFF
// [Substitutiong] nothing
// [return] nothing
///////////////////////////////////////////////////////////////////////
void setIrledPwm( uint8_t able )
{
	ctr_irled = able;
}


void setSensorConstant( sensor_t *sen, int16_t reference, int16_t threshold )
{
  sen->reference = reference;
  sen->threshold = threshold;
}

void update_sensor_data( void )
{

  sen_front.now = ( sen_fl.now + sen_fr.now ) / 2;

  if ( sen_front.now < sen_front.threshold ){
    sen_front.is_wall = false;
  } else {
    sen_front.is_wall = true;
  }

  if ( sen_l.now < sen_l.threshold ){
    sen_l.is_wall = false;
  } else {
    sen_l.is_wall = true;
  }

  if ( sen_r.now < sen_r.threshold ){
    sen_r.is_wall = false;
  } else {
    sen_r.is_wall = true;
  }
}

void adcStart( void )
{
  setIrledPwm( IRLED_ON );
  adc_counter = 0;
  HAL_ADC_Start_DMA( &hadc1, (uint32_t *)ADCBuff, sizeof(ADCBuff) );
}
/*
void adcEnd( void )
{
  setIrledPwm( IRLED_OFF );
}
*/
void adcCheckConvert( void )
{
  if ( adc_counter == FINISH_CONVERT && ctr_irled == 1 ){
    update_sensor_data();
    adc_counter = 0;
    HAL_ADC_Start_DMA( &hadc1, (uint32_t *)ADCBuff, sizeof(ADCBuff) );
  }
}

// DMA の変換式を記載
void getADSensor( int16_t *adcount )
{
  volatile int i;
  switch( *adcount ) {
    case OFF_VALUE:
      HAL_ADC_Stop_DMA( &hadc1 );
      ADCOffData[0] = ADCBuff[0];
      ADCOffData[1] = ADCBuff[1];
      ADCOffData[2] = ADCBuff[2];
      ADCOffData[3] = ADCBuff[3];

      HAL_GPIO_WritePin( paluse2_GPIO_Port, paluse2_Pin ,GPIO_PIN_SET );
      HAL_GPIO_WritePin( paluse3_GPIO_Port, paluse3_Pin ,GPIO_PIN_SET );
      for( i = 0; i < 100; i++ ){

      }

      *adcount = LEFT_VALUE;

      HAL_ADC_Start_DMA( &hadc1, (uint32_t *)ADCBuff, sizeof(ADCBuff) );
      break;

    case LEFT_VALUE:
      HAL_GPIO_WritePin( paluse2_GPIO_Port, paluse2_Pin ,GPIO_PIN_RESET );
      HAL_GPIO_WritePin( paluse3_GPIO_Port, paluse3_Pin ,GPIO_PIN_RESET );
      HAL_ADC_Stop_DMA( &hadc1 );

      ADCOntData[2] = ADCBuff[2];
      ADCOntData[3] = ADCBuff[3];

      sen_l.diff = sen_l.now - (ADCOntData[2] - ADCOffData[2]);
      sen_l.now = ADCOntData[2] - ADCOffData[2];

      sen_fl.diff = sen_fl.now - (ADCOntData[3] - ADCOffData[3]);
      sen_fl.now = ADCOntData[3] - ADCOffData[3];

      HAL_GPIO_WritePin( paluse0_GPIO_Port, paluse0_Pin ,GPIO_PIN_SET );
      HAL_GPIO_WritePin( paluse1_GPIO_Port, paluse1_Pin ,GPIO_PIN_SET );
      for( i = 0; i < 100; i++ ){

      }

      *adcount = RIGHT_VALUE;

      HAL_ADC_Start_DMA( &hadc1, (uint32_t *)ADCBuff, sizeof(ADCBuff) );
      break;

    case RIGHT_VALUE:
      HAL_GPIO_WritePin( paluse0_GPIO_Port, paluse0_Pin ,GPIO_PIN_RESET );
      HAL_GPIO_WritePin( paluse1_GPIO_Port, paluse1_Pin ,GPIO_PIN_RESET );
      HAL_ADC_Stop_DMA( &hadc1 );

      ADCOntData[0] = ADCBuff[0];
      ADCOntData[1] = ADCBuff[1];

      sen_fr.diff = sen_fr.now - (ADCOntData[0] - ADCOffData[0]);
      sen_fr.now = ADCOntData[0] - ADCOffData[0];

      sen_r.diff = sen_r.now - (ADCOntData[1] - ADCOffData[1]);
      sen_r.now = ADCOntData[1] - ADCOffData[1];

      *adcount = FINISH_CONVERT;
      break;

    default:
      break;
  }
}
