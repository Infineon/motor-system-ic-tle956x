/** 
 * @file        logger-arduino.hpp
 * @brief       TLE9563 Logger Arduino PAL 
 * @date        January 2021
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_PAL_LOGGER_INO_H_
#define TLE9563_PAL_LOGGER_INO_H_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

#include "../../../pal/logger.hpp"

#if (TLE9563_LOGGER_ENABLED == 1)

class LoggerIno : public LoggerPAL
{
        public:
                        LoggerIno    ();
                       ~LoggerIno    (); 
            Error_t     init         ();
            Error_t     deinit       ();
            Error_t     write        (const uint8_t * log_data, uint32_t length);
};

/**
 * @brief Default LoggerIno
 */
extern LoggerIno logpalino;

#endif /* TLE9563_LOGGER_ENABLED */
#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_PAL_LOGGER_INO_H_ **/