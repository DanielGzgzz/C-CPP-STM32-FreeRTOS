/*
 * cpp_adc_mapping.cpp
 *
 *  Created on: Sep 18, 2023
 *      Author: daniel
 *
 *      That is the CPP code in the project.
 */


#include "cpp_adc_mapping.hpp"

// Constants for ADC value range
constexpr int ADC_MIN = 0;
constexpr int ADC_MAX = 4095;

// Constants for frequency range
constexpr int MIN_FREQUENCY = 200;  // Minimum frequency in Hz
constexpr int MAX_FREQUENCY = 20000; // Maximum frequency in Hz

// Variables for mapped values
float normalizedX;
float normalizedY;
float calculated_angle;
uint16_t frequency;

// Function to map ADC values to frequency
int MapToFrequency(uint16_t xValue, uint16_t yValue) {
    // Normalize ADC values to the range [0, 1]
    normalizedX = static_cast<float>(xValue - ADC_MIN) / (ADC_MAX - ADC_MIN);
    normalizedY = static_cast<float>(yValue - ADC_MIN) / (ADC_MAX - ADC_MIN);

    // Calculate angle based on joystick position (0 to 2*pi)
    calculated_angle = 2 * M_PI * normalizedX;

    // Calculate frequency based on joystick position and range
    frequency = static_cast<uint16_t>(MIN_FREQUENCY + (MAX_FREQUENCY - MIN_FREQUENCY) * normalizedY +
                                      50 * std::sin(calculated_angle)); // Adjust the modulation depth
    return frequency;
}
