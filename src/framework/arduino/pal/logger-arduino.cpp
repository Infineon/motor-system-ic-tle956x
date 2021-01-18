/** 
 * @file        logger-arduino.cpp
 * @brief       TLE9563 Logger Arduino PAL 
 * @date        January 2021
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */


#include "logger-arduino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

#if (TLE9563_LOGGER_ENABLED == 1)

#include <Arduino.h>
#include "../../../corelib/BLDCM-logger.hpp"

/**
 * @brief Logger Arduino PAL constructor
 */
LoggerIno::LoggerIno()
{

}

/**
 * @brief Logger Arduino PAL destructor
 */
LoggerIno::~LoggerIno()
{

}

/**
 * @brief       Initializes the logger Arduino PAL 
 * @note        Serial is initialized by the Arduino core 
 *              and begun in setup().
 * @return      Logger PAL error code
 * @retval      OK always
 */
LoggerPAL::Error_t  LoggerIno::init()
{
  return LoggerPAL::Error_t::OK;
}

/**
 * @brief       Deinitializes the logger Arduino PAL
 * @return      Logger PAL error code
 * @retval      OK always
 */
LoggerPAL::Error_t   LoggerIno::deinit()
{
  return LoggerPAL::Error_t::OK;
} 

/**
 * @brief       Writes in the logger Arduino PAL
 * @param[in]   *logData    Pointer to the array holding the data to be logged
 * @param[in]   length      Length in bytes of the data to be written 
 * @return      Logger PAL error code
 * @retval      OK always
 */
LoggerPAL::Error_t   LoggerIno::write(const uint8_t * log_data, uint32_t length)
{
  String str = (char*)log_data;

  for(uint32_t i=0; i< length; i++) {
    Serial.print(str.charAt(i));
  }
  return LoggerPAL::Error_t::OK;
}

/**
 * @brief Library Logger
 */
LoggerIno logpalino;
Logger    tle9563_log(static_cast<LoggerPAL*>(&logpalino));


#endif /* TLE9563_LOGGER_ENABLED */
#endif /** TLE9563_FRAMEWORK **/