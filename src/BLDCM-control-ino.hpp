/*!
 * @file        BLDCM-control-ino.hpp
 * @name        BLDCM-control-ino.hpp - basic motor control functions for Arduino
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * @ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BLDCMCONTROLINO_HPP_
#define BLDCMCONTROLINO_HPP_

#include "config/tle9xxx-conf.hpp"
#include "corelib/BLDCM-control.hpp"

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
 * @class BLDCMcontrolIno
 *
 * @brief represents the BLDCMcontrol base class
 *
 */
class BLDCMcontrolIno: public  BLDCMcontrol
{
	public:
		/**
		 * @brief create and assign Arduino GPIOs and PWM pins
		 * 
		 */
		BLDCMcontrolIno(void);

};

/** @} */
#endif	/** MOTOR_SYSTEM_IC_FRAMEWORK **/
#endif 	/** BLDCMCONTROLINO_HPP_ **/