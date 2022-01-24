/** 
 * @file        logger-arduino.hpp
 * @brief       TLE9563 Logger Arduino PAL 
 * @copyright   2022 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_PAL_LOGGER_INO_H_
#define TLE9563_PAL_LOGGER_INO_H_

#include "../../../config/tle9xxx-conf.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)

#include "../../../pal/logger.hpp"

#if (TLE9XXX_LOGGER_ENABLED == 1)

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

#endif /* TLE9XXX_LOGGER_ENABLED */
#endif /** MOTOR_SYSTEM_IC_FRAMEWORK **/
#endif /** TLE9563_PAL_LOGGER_INO_H_ **/