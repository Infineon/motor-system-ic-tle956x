/*!
 * \file        TLE9563-ino.hpp
 * \name        TLE9563-ino.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \ref         tle9563ino
 * \version     1.0.0
 * \brief       This is a Arduino platform file use Infineon's BLDC Motor Control Shield with TLE9563 with
 *              Arduino/Genuino, Infineon XMC microcontroller or compatible boards.
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_INO_HPP_
#define TLE9563_INO_HPP_

#include "config/tle9563-conf.hpp"
#include "corelib/TLE9563.hpp"

#include "framework/arduino/pal/adc-arduino.hpp"
#include "framework/arduino/pal/gpio-arduino.hpp"
#include "framework/arduino/pal/spic-arduino.hpp"
#include "framework/arduino/pal/timer-arduino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/**
 * @addtogroup inoApi
 * @{
 */
#include "framework/arduino/wrapper/pin-config-arduino.hpp"

/**
 * @class Tle9563Ino
 *
 * @brief represents the TLE9563 base class
 *
 * This class provides a simple API for connecting the TLE9563 shield via SPI to an Arduino.
 * The three PWM pins, BEMF input Pins and hallsensor input pins are created and assigned to Arduino Hardware in here.
 *
 */
class TLE9563Ino: virtual public Tle9563
{
	public:
		/**
		 * @brief create and assign Arduino GPIOs and PWM pins
		 * 
		 */
		TLE9563Ino(void);
};
/* @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_INO_HPP_ **/
