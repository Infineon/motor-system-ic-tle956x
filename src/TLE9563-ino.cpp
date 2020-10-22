/*!
 * \file        TLE9563-ino.cpp
 * \name        TLE9563.ino.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
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
#include "framework/arduino/pal/adc-arduino.hpp"
#include "framework/arduino/pal/gpio-arduino.hpp"
#include "framework/arduino/pal/spic-arduino.hpp"
#include "framework/arduino/pal/timer-arduino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/*! \brief SPI address commands */
#define TLE9563_CMD_WRITE          0x80
#define TLE9563_CMD_CLEAR          0x80

/*! \brief time in milliseconds to wait for chipselect signal raised */
#define TLE9563_CS_RISETIME        2

TLE9563Ino::TLE9563Ino(void)
{
	Tle9563::intn = new GPIOIno(ARDUINO_UNO.INTN, INPUT, GPIOIno::POSITIVE );
	Tle9563::cso = new ADCIno(ARDUINO_UNO.CSO);

	Tle9563::pwmU = new ADCIno(ARDUINO_UNO.PWM_U);
	Tle9563::pwmV = new ADCIno(ARDUINO_UNO.PWM_V);
	Tle9563::pwmW = new ADCIno(ARDUINO_UNO.PWM_W);

	Tle9563::bemfU = new GPIOIno(ARDUINO_UNO.BEMF_U_IO, INPUT, GPIOIno::POSITIVE );
	Tle9563::bemfV = new GPIOIno(ARDUINO_UNO.BEMF_V_IO, INPUT, GPIOIno::POSITIVE );
	Tle9563::bemfW = new GPIOIno(ARDUINO_UNO.BEMF_W_IO, INPUT, GPIOIno::POSITIVE );

	Tle9563::hallA = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );
	Tle9563::hallB = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );
	Tle9563::hallC = new GPIOIno(ARDUINO_UNO.HALL_A, INPUT, GPIOIno::POSITIVE );

	Tle9563::csn = new GPIOIno(ARDUINO_UNO.CSN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::timer = new TimerIno();
	Tle9563::sBus = new SPICIno(LSBFIRST, SPI_MODE1, SPI_CLOCK_DIV2);
	//Tle9563::sBus = new SPICIno();
}

void TLE9563Ino::begin(void)
{
	//mEnabled = false;
	Tle9563::sBus->init();
	Tle9563::csn->init();
	Tle9563::csn->enable();

	Tle9563::intn->init();
	Tle9563::cso->init();

	Tle9563::pwmU->init();
	Tle9563::pwmV->init();
	Tle9563::pwmW->init();
	Tle9563::pwmU->setWriteFrequency(1);
	Tle9563::pwmV->setWriteFrequency(1);
	Tle9563::pwmW->setWriteFrequency(1);

	Tle9563::bemfU->init();
	Tle9563::bemfV->init();
	Tle9563::bemfW->init();
	Tle9563::hallA->init();
	Tle9563::hallB->init();
	Tle9563::hallC->init();

	Tle9563::timer->init();
	//mEnabled = true;
}

void TLE9563Ino::end(void)
{
	//mEnabled = false;
	Tle9563::intn->deinit();
	Tle9563::cso->deinit();

	Tle9563::pwmU->deinit();
	Tle9563::pwmV->deinit();
	Tle9563::pwmW->deinit();
	Tle9563::bemfU->deinit();
	Tle9563::bemfV->deinit();
	Tle9563::bemfW->deinit();
	Tle9563::hallA->deinit();
	Tle9563::hallB->deinit();
	Tle9563::hallC->deinit();

	Tle9563::csn->deinit();
	Tle9563::sBus->deinit();
	Tle9563::timer->stop();
}

void TLE9563Ino::SBC_CRC_Disable()
{
	uint8_t byte0;
	csn->disable();
	sBus->transfer(0xe7, byte0);	// 0b 1110 0111
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0xc3, byte0);	// 0b 1100 0011
	csn->enable();
	//timer->delayMilli(TLE9563_CS_RISETIME);
}

void TLE9563Ino::SBC_SPI(uint8_t addr, uint16_t data)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9563_CMD_WRITE), byte0);	// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer((data&0xFF), byte1);					// lower 8 bit of data
	sBus->transfer(((data>>8)&0xFF), byte2);			// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
	//timer->delayMilli(TLE9563_CS_RISETIME);
}

#endif /** TLE9563_FRAMEWORK **/
