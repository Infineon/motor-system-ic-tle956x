/*!
 * \file        TLE9563Motor-ino.hpp
 * \name        TLE9563Motor-ino.hpp - optionally include library
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     2.0.0
 * \ref         tle9563ino
 * \brief       This file can optionally be included in projects that use an Infineon
 *              DC Motor Control Shield with TLE9563
 *              It provides a higher abstraction for controlling motors with the TLE9563
 *              acting as an output driver
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563MOTOR_INO_HPP_
#define TLE9563MOTOR_INO_HPP_

#include "config/tle9563-conf.hpp"

/**
 * @addtogroup inoApi
 * @{
 */

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)


#include "TLE9563-ino.hpp"
#include "framework/arduino/wrapper/TLE9563Motor-platf-ino.hpp"

/**
 * @class Tle9563MotorIno
 *
 * @brief represents a motor connected to a TLE9563
 *
 * This class provides a simple API for connecting and controlling motors.
 * Each motor is assigned to a Tle9563 which acts as output driver. Calls to
 * Tle9563Motor instances are mapped to calls to Tle9563. Therefore, this
 * class does not bring new features, it does only provide further abstraction.
 *
 * @see Tle9563
 */
class Tle9563MotorIno: public Tle9563Motor
{
};

/**
 * @}
 */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563MOTOR_INO_HPP_ **/
