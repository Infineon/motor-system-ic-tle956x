/*!
 * @file        DCM-control-ino.hpp
 * @name        DCM-control-ino.hpp - basic motor control functions for Arduino
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to control DC motors using an instance of TLE9562
 * @ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef DCMCONTROLINO_HPP_
#define DCMCONTROLINO_HPP_

#include "config/tle9xxx-conf.hpp"
#include "corelib/DCM-control.hpp"

#include "framework/arduino/pal/adc-arduino.hpp"
#include "framework/arduino/pal/gpio-arduino.hpp"
#include "framework/arduino/pal/spic-arduino.hpp"
#include "framework/arduino/pal/timer-arduino.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)

/**
 * @addtogroup inoApi
 * @{
 */
#include "framework/arduino/wrapper/pin-config-arduino.hpp"

/**
 * @class DCMcontrolIno
 *
 * @brief represents the DCMcontrol base class
 *
 */
class DCMcontrolIno: public  DCMcontrol
{
	public:
		/**
		 * @brief create and assign Arduino GPIOs and PWM pins
		 * 
		 */
		DCMcontrolIno(void);

};

/** @} */
#endif	/** MOTOR_SYSTEM_IC_FRAMEWORK **/
#endif 	/** DCMCONTROLINO_HPP_ **/