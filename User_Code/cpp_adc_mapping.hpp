/*
 * cpp_adc_mapping.hpp
 *
 *  Created on: Sep 18, 2023
 *      Author: daniel
 */

// cpp_adc_mapping.hpp

#ifndef CPP_ADC_MAPPING_HPP_
#define CPP_ADC_MAPPING_HPP_

#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include "main.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"



int MapToFrequency(uint16_t xValue, uint16_t yValue); // Declare a C++ function

#ifdef __cplusplus
}
#endif

#endif /* CPP_ADC_MAPPING_HPP_ */
