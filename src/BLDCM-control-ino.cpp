/*!
 * \file        BLDCM-control-ino.cpp
 * \name        BLDCM-control-ino.cpp - basic motor control functions for Arduino
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "BLDCM-control-ino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)


BLDCMcontrolIno::BLDCMcontrolIno(void)
{
	BLDCMcontrol::pwmU = new ADCIno(ARDUINO_UNO.PWM_U);
	BLDCMcontrol::pwmV = new ADCIno(ARDUINO_UNO.PWM_V);
	BLDCMcontrol::pwmW = new ADCIno(ARDUINO_UNO.PWM_W);

	BLDCMcontrol::bemfU = new GPIOIno(ARDUINO_UNO.BEMF_U_IO, INPUT, GPIOIno::POSITIVE );
	BLDCMcontrol::bemfV = new GPIOIno(ARDUINO_UNO.BEMF_V_IO, INPUT, GPIOIno::POSITIVE );
	BLDCMcontrol::bemfW = new GPIOIno(ARDUINO_UNO.BEMF_W_IO, INPUT, GPIOIno::POSITIVE );

	BLDCMcontrol::hallA = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );
	BLDCMcontrol::hallB = new GPIOIno(ARDUINO_UNO.HALL_B, INPUT, GPIOIno::POSITIVE );
	BLDCMcontrol::hallC = new GPIOIno(ARDUINO_UNO.HALL_C, INPUT, GPIOIno::POSITIVE );

	BLDCMcontrol::timer = new TimerIno();
	BLDCMcontrol::rpmtimer = new TimerIno();
	
	BLDCMcontrol::controller = new TLE9563Ino();
}



/** @} */
#endif	/** TLE9563_FRAMEWORK **/