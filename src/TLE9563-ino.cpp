/*!
 * \file        TLE9563-ino.cpp
 * \name        TLE9563.ino.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \ref         tle9563ino
 * \brief       This file has to be included in projects that use Infineon's BLDC Motor Control Shield with TLE9563
 * \details
 * 
 * This file defines the functions for the Arduino-Inheritance of TLE9563 class. It assigns the Arduino pins to the virtual Pin definitions.
 */


#include "TLE9563-ino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

TLE9563Ino::TLE9563Ino(void)
{
	Tle9563::intn = new GPIOIno(ARDUINO_UNO.INTN, INPUT, GPIOIno::POSITIVE );
	Tle9563::cso = new ADCIno(ARDUINO_UNO.CSO);

	Tle9563::csn = new GPIOIno(ARDUINO_UNO.CSN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::sBus = new SPICIno(LSBFIRST, SPI_MODE1, SPI_CLOCK_DIV2);

	Tle9563::timer = new TimerIno();
}

#endif /** TLE9563_FRAMEWORK **/
