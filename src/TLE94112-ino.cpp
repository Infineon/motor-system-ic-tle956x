/*!
 * \file        TLE9563-ino.cpp
 * \name        TLE9563.ino.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     2.0.0
 * \ref         tle9563ino
 * \brief       This file has to be included in projects that use Infineon's DC Motor Control Shield with TLE9563
 * \details
 * The Infineon TLE9563EL DC motor controller shield is able to handle 6 motors with a max. current of 0.9 A
 * independently and additional 5 motors cascaded. The twelve half-bridges can be arranged also together,
 * so that 3 motors with 1.8 A current or one motor with 3.6 A can be used. Each half bridge can
 * provide a high-Voltage (nominal 5.5-18 V) tristate output and max. input voltage of 40V. It is also
 * capable of PWM with 3 different frequencies for controlling the speed of each motor.
 * Have a look at the datasheet for more information.
 * - This library include the basic functions to access the half-bridges.
 * - Have a look at the datasheet for more information.
 */


#include <TLE9563-ino.hpp>

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

Tle9563Ino::Tle9563Ino(void):Tle9563()
{
	Tle9563::en = new GPIOIno(ARDUINO_UNO.INTN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::cs = new GPIOIno(ARDUINO_UNO.CSN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::timer = new TimerIno();
	Tle9563::sBus = new SPICIno();
}

Tle9563Ino::Tle9563Ino(SPIClass &bus, uint8_t csPin):Tle9563()
{
	Tle9563::en = new GPIOIno(TLE9563_PIN_EN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::cs = new GPIOIno(csPin, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::timer = new TimerIno();
	Tle9563::sBus = new SPICIno(bus,csPin,MISO,MOSI,SCK);
}

void Tle9563Ino::begin(void)
{
	mEnabled = false;
	Tle9563::sBus->init();
	Tle9563::en->init();
	Tle9563::en->enable();
	Tle9563::cs->init();
	Tle9563::cs->enable();
	Tle9563::timer->init();
	mEnabled = true;
	init();
}

void Tle9563Ino::end(void)
{
	mEnabled = false;
	Tle9563::en->disable();
	Tle9563::cs->disable();
	Tle9563::timer->stop();
	Tle9563::sBus->deinit();
}

#endif /** TLE9563_FRAMEWORK **/
