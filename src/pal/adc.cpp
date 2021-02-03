/** 
 * @file        adc.cpp
 * @brief       PAL of the ADC 
 * @date        May 2020
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "adc.hpp"

AnalogDigitalConverter::Error_t AnalogDigitalConverter::checkErrorStatus()
{
    return errorStatus;
}