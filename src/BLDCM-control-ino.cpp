/*!
 * \file        BLDCM-control-ino.cpp
 * \name        BLDCM-control-ino.cpp - basic motor control functions for Arduino
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
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
	BLDCMcontrol::hallB = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );
	BLDCMcontrol::hallC = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );

	BLDCMcontrol::timer = new TimerIno();
}

void BLDCMcontrolIno::begin(void)
{
	BLDCMcontrol::pwmU->init();
	BLDCMcontrol::pwmV->init();
	BLDCMcontrol::pwmW->init();
	BLDCMcontrol::pwmU->setWriteFrequency(1);
	BLDCMcontrol::pwmV->setWriteFrequency(1);
	BLDCMcontrol::pwmW->setWriteFrequency(1);

	BLDCMcontrol::bemfU->init();
	BLDCMcontrol::bemfV->init();
	BLDCMcontrol::bemfW->init();

	BLDCMcontrol::hallA->init();
	BLDCMcontrol::hallB->init();
	BLDCMcontrol::hallC->init();

	BLDCMcontrol::timer->init();
}

void BLDCMcontrolIno::end(void)
{
	BLDCMcontrol::pwmU->deinit();
	BLDCMcontrol::pwmV->deinit();
	BLDCMcontrol::pwmW->deinit();

	BLDCMcontrol::bemfU->deinit();
	BLDCMcontrol::bemfV->deinit();
	BLDCMcontrol::bemfW->deinit();

	BLDCMcontrol::hallA->deinit();
	BLDCMcontrol::hallB->deinit();
	BLDCMcontrol::hallC->deinit();

	BLDCMcontrol::timer->stop();
}

/** @} */
#endif	/** TLE9563_FRAMEWORK **/