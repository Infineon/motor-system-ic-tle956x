/** 
 * @file        adc-arduino.hpp
 * @brief       Arduino PAL for the ADC
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef ADC_ARDUINO_H_
#define ADC_ARDUINO_H_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

#include "../../../corelib/TLE9563.hpp"

/**
 * @addtogroup arduinoPal
 * @{
 */


/**
 * @brief Arduino ADC class
 * 
 */
class ADCIno : virtual public AnalogDigitalConverter
{
    private:
    uint8_t pin;

    public:
                ADCIno();
                ADCIno(uint8_t pin);
                ~ADCIno();
    Error_t     init();
    Error_t     deinit();
    Error_t     enable();
    Error_t     disable();
    Error_t     setReadResolution(uint8_t resolution);
    Error_t     setWriteResolution(uint8_t resolution);
    Error_t     setWriteFrequency(uint32_t divisor);
    uint16_t    ADCRead();
    Error_t     ADCWrite(uint8_t value);
};
/** @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** ADC_ARDUINO_H_ **/
