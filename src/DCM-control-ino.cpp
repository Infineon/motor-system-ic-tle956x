/*!
 * \file        DCM-control-ino.cpp
 * \name        DCM-control-ino.cpp - basic motor control functions for Arduino
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic common functions to control DC motors using an instance of TLE9562
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "DCM-control-ino.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)


DCMcontrolIno::DCMcontrolIno(void)
{
	DCMcontrol::pwmA = new ADCIno(ARDUINO_UNO.PWM_U);
	DCMcontrol::pwmB = new ADCIno(ARDUINO_UNO.PWM_V);

	DCMcontrol::timer = new TimerIno();
	
	DCMcontrol::controller = new Tle9562();

	controller->intn = new GPIOIno(ARDUINO_UNO.INTN, INPUT, GPIOIno::POSITIVE );

	controller->csn = new GPIOIno(ARDUINO_UNO.CSN, OUTPUT, GPIOIno::POSITIVE );
	controller->sBus = new SPICIno(LSBFIRST, SPI_MODE1, SPI_CLOCK_DIV2);

	controller->timer = new TimerIno();
}



/** @} */
#endif	/** MOTOR_SYSTEM_IC_FRAMEWORK **/