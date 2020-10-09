/** 
 * @file        pin-config-arduino.cpp
 * @brief       Configurations for the different Platfroms
 * @date        Sept 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pin-config-arduino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include <Arduino.h>

hardwareconfig_t ARDUINO_UNO
{
    .INTN = 2,                      //Interrupt input / Test

    .PWM_U = 3,                     //PWM1
    .PWM_V = 9,                     //PWM3
    .PWM_W = 10,                    //PWM5

    .BEMF_U_IO = 5,                 //BEMF input from comparator U
    .BEMF_V_IO = 6,                 //BEMF input from comparator V
    .BEMF_W_IO = 7,                 //BEMF input from comparator W

    .CSN = 8,                       //SPI Ship Select Input
    .SDI = 11,                      //SPI Data output to TLE (=MOSI)
    .SDO = 12,                      //SPI Data input from TLE (=MISO)
    .CLK = 13,                      //SPI Clock output

    .CSO = A5,                      //Current sense amplifier input

    .HALL_A = A2,                   //Hall sensor input
    .HALL_B = A1,                   //Hall sensor input
    .HALL_C = A0                    //Hall sensor input
};

#endif /** TLE9563_FRAMEWORK **/