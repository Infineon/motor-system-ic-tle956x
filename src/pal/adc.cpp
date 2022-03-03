/** 
 * @file        adc.cpp
 * @brief       PAL of the ADC 
 * @copyright   2022 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "adc.hpp"

AnalogDigitalConverter::Error_t AnalogDigitalConverter::checkErrorStatus()
{
    return errorStatus;
}