/** 
 * @file        pin-config-arduino.hpp
 * @brief       Configurations for the different platfroms
 * @date        Sept 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PINCONFIGARDUINO_HPP_
#define PINCONFIGARDUINO_HPP_

#include <stdint.h>

/**
 * @addtogroup arduinoWrapper
 * @{
 */


/**
 * @brief Struct with all necessary defines of the TLE956x BLDCM shield
 * 
 */
struct hardwareconfig_t
{
    uint8_t INTN;

    uint8_t PWM_U;
    uint8_t PWM_V;
    uint8_t PWM_W;

    uint8_t BEMF_U_IO;
    uint8_t BEMF_V_IO;
    uint8_t BEMF_W_IO;

    uint8_t CSN;
    uint8_t SDI;
    uint8_t SDO;
    uint8_t CLK;

    uint8_t CSO;

    uint8_t HALL_A;
    uint8_t HALL_B;
    uint8_t HALL_C;    
};

extern hardwareconfig_t ARDUINO_UNO;
/** @} */

#endif /** PINCONFIGARDUINO_HPP_ **/