/*
 * main_logic.hpp
 *
 *  Created on: Sep 18, 2023
 *      Author: daniel
 */
// main_logic.h
#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx_hal.h"
#include <math.h>
#include <string.h> // For string functions
#include <stdio.h> // For snprintf

#include "cpp_adc_mapping.hpp"




/*Thread block*/
extern osThreadId ADC_To_PWM_ConvHandle;
extern uint32_t ADC_To_PWM_Conv_Buff[ 128 ];
extern osStaticThreadDef_t ADC_To_PWM_Conv_ControlBlock;
void ADC_To_PWM_Conv_Start(void const *argument);

/*Thread Block End*/


extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart3;


extern uint8_t volatile adcCpltInterruptFlag1;
extern uint8_t volatile adcCpltInterruptFlag2;
extern uint8_t playSoundBool;

extern uint16_t volatile adc1Value;
extern uint16_t volatile adc2Value;

extern uint8_t pwm_down_flag;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void Set_PWM_Frequency(uint32_t frequency);



#ifdef __cplusplus
}
#endif

#endif // MAIN_LOGIC_H
