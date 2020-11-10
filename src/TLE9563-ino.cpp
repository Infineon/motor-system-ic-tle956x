/*!
 * \file        TLE9563-ino.cpp
 * \name        TLE9563.ino.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \ref         tle9563ino
 * \brief       This file has to be included in projects that use Infineon's BLDC Motor Control Shield with TLE9563
 * \details
 * 
 * This file defines the functions for the Arduino-Inheritance of TLE9563 class. It assigns the Arduino pins to the virtual Pin definitions
 * and inits the Hardware like GPIO, timer and SPI. Moreover, the SPI send functions are defined here as well.
 */


#include "TLE9563-ino.hpp"

/*! \brief SPI address commands */
#define TLE9563_CMD_WRITE          	0x80
#define TLE9563_CMD_READ          	0x00
#define TLE9563_CMD_CLEAR          	0x80

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

TLE9563Ino::TLE9563Ino(void)
{
	Tle9563::intn = new GPIOIno(ARDUINO_UNO.INTN, INPUT, GPIOIno::POSITIVE );
	Tle9563::cso = new ADCIno(ARDUINO_UNO.CSO);

	Tle9563::csn = new GPIOIno(ARDUINO_UNO.CSN, OUTPUT, GPIOIno::POSITIVE );
	Tle9563::sBus = new SPICIno(LSBFIRST, SPI_MODE1, SPI_CLOCK_DIV2);

	Tle9563::timer = new TimerIno();
}

void TLE9563Ino::begin(void)
{
	//mEnabled = false;
	Tle9563::intn->init();
	Tle9563::cso->init();
	
	Tle9563::csn->init();
	Tle9563::csn->enable();
	Tle9563::sBus->init();

	Tle9563::timer->init();
	//mEnabled = true;
}

void TLE9563Ino::end(void)
{
	//mEnabled = false;
	Tle9563::intn->deinit();
	Tle9563::cso->deinit();

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
}

void TLE9563Ino::writeReg(uint8_t addr, uint16_t data)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9563_CMD_WRITE), byte0);	// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer((data&0xFF), byte1);					// lower 8 bit of data
	sBus->transfer(((data>>8)&0xFF), byte2);			// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
}

uint16_t TLE9563Ino::readReg(uint8_t addr)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9563_CMD_READ), byte0);		// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer(0xFF, byte1);						// lower 8 bit of data
	sBus->transfer(0xFF, byte2);						// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
	return ((byte2<<8)|byte1);
}

#endif /** TLE9563_FRAMEWORK **/
