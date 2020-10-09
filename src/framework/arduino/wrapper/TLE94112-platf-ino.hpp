/**
 * @file        TLE9563-platf-ino.hpp
 * @brief       TLE9563-platf-ino.hpp Arduino Hardware Platforms
 * @date        Mai 2020
 * @version     2.0.0
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_PLATF_INO_HPP_
#define TLE9563_PLATF_INO_HPP_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

#include "../pal/TLE9563-pal-ino.hpp"

/**
 * @addtogroup platfIno
 * @{
 *
 * @brief Arduino Hardware Platform Pins
 */

/*!
 * Standard chip select pin for first TLE9563 shield
 */
#define TLE9563_PIN_CS1     10

/*!
 * Standard chip select pin for second TLE9563 shield
 * To use a second shield with different CS pin you have
 * to remove the 0 Ohm resistor R7 and solder it to R8
 */
#define TLE9563_PIN_CS2     9

/*!
 * Standard TLE9563 enable pin
 */
#define TLE9563_PIN_EN      8

/**
 * @}
 */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_PLATF_INO_HPP_ **/