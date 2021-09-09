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

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)


DCMcontrolIno::DCMcontrolIno(void)
{
	DCMcontrol::pwmA = new ADCIno(ARDUINO_UNO.PWM_U);
	DCMcontrol::pwmB = new ADCIno(ARDUINO_UNO.PWM_V);

	DCMcontrol::timer = new TimerIno();
	
	DCMcontrol::controller = new TLE9563Ino();
}



/** @} */
#endif	/** TLE9563_FRAMEWORK **/