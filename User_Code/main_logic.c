/*
 * main_logic.cpp
 *
 *  Created on: Sep 18, 2023
 *      Author: daniel
 */


// main_logic.c
#include "main_logic.h"
#include "stm32f7xx_hal.h" // Include necessary headers
/**
 * Variables defined here and not at "main_logic.h" to not
 * collide with the inclusion of "main_logic.h" causng compilation errors
 */
osThreadId ADC_To_PWM_ConvHandle;
uint32_t ADC_To_PWM_Conv_Buff[128];
osStaticThreadDef_t ADC_To_PWM_Conv_ControlBlock;

uint8_t volatile adcCpltInterruptFlag1;
uint8_t volatile adcCpltInterruptFlag2;
uint8_t playSoundBool;

uint16_t volatile adc1Value;
uint16_t volatile adc2Value;
uint8_t pwm_down_flag;
/*Basically an unused Callback and unimplemented feature of joystick button.*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == PF3_JoystickButton_Pin) {
		playSoundBool = !playSoundBool;
	}
}
/* Callback _weak override in C  Used for the FreeRTOS Thread "ADC_TO_PWM" checks flags by polling .*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == &hadc1) {
		adcCpltInterruptFlag1 = 1;
		// Additional processing for ADC1
	} else if (hadc == &hadc2) {
		adcCpltInterruptFlag2 = 1;
		// Additional processing for ADC2
	}
	// You can add more conditions for other ADCs if needed
}

/* Callback _weak override in C , Used
 *  to shift frequency without causing accidentally a high
 *  frequency beep noise due cutting on working cycle to the buzzer
 * */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {

	if(adcCpltInterruptFlag1 && adcCpltInterruptFlag2){
		HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_1);
		pwm_down_flag=1;
	}

	if (htim == &htim3) {


		pwm_down_flag = 1;
		}


}
/*
 * Function that sets a frequency to the PWM generation channel.
 * */
void Set_PWM_Frequency(uint32_t frequency) {
	//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // Stop PWM

	htim3.Init.Period = HAL_RCC_GetHCLKFreq() / (frequency) - 1; // Calculate new period

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		// Turn on the red LED to indicate failure
		// HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_SET);
		Error_Handler();
	} else {
		// Turn on the green LED to indicate success
		HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET);

		// Initialize PWM channel with new period
		TIM_OC_InitTypeDef sConfigOC = { 0 };
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = htim3.Init.Period / 2; // 50% duty cycle initially
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

		//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Restart PWM
	}
}


/**
 * @brief Function implementing the ADC_To_PWM_Conv thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ADC_To_PWM_Conv_Start */
void ADC_To_PWM_Conv_Start(void const *argument) {
	/* USER CODE BEGIN ADC_To_PWM_Conv_Start */
	/* Infinite loop */
	//Set_PWM_DutyCycle(100);
	Set_PWM_Frequency(2000);
	if (HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1) == HAL_OK) {
	}

	for (;;) {

		if (adcCpltInterruptFlag1 && adcCpltInterruptFlag2 && pwm_down_flag) {
			// Read value.
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);

			adc1Value = HAL_ADC_GetValue(&hadc1);
			adc2Value = HAL_ADC_GetValue(&hadc2);


			Set_PWM_Frequency(MapToFrequency(adc1Value,adc2Value));
			adcCpltInterruptFlag1 = 0;
			adcCpltInterruptFlag2 = 0;

			HAL_ADC_Start_IT(&hadc1);
			HAL_ADC_Start_IT(&hadc2);
			pwm_down_flag = 0;
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);


		}


	}

}


