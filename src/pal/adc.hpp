/** 
 * @file        adc.hpp
 * @brief       PAL of the ADC 
 * @date        May 2020
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>

#ifndef AnalogDigitalConverter_HPP_
#define AnalogDigitalConverter_HPP_


/**
 * @addtogroup hssPal 
 * @{
 */

class AnalogDigitalConverter
{
    public:

        enum Error_t
        {
            OK          =  0,    /**< No error */
            INTF_ERROR  = -1,    /**< Interface error */
            CONF_ERROR  = -2,    /**< Configuration error */
            READ_ERROR  = -3,    /**< Read error */
            WRITE_ERROR = -4,    /**< Write error */
        };

        virtual Error_t     init() = 0;
        virtual Error_t     deinit() = 0;
        virtual Error_t     enable() = 0;
        virtual Error_t     disable() = 0;
        virtual Error_t     setReadResolution(uint8_t resolution) = 0;
        virtual Error_t     setWriteResolution(uint8_t resolution) = 0;
        virtual Error_t     setWriteFrequency(uint32_t frequency) = 0;
        virtual uint16_t    ADCRead() = 0;
        virtual Error_t     ADCWrite(uint8_t value) = 0;
        Error_t             checkErrorStatus();

        private:

        Error_t errorStatus;
};
/** @} */

#endif /** AnalogDigitalConverter_HPP_ **/


