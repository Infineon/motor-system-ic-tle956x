/*!
 * \file        TLE9563-pal-wiced.cpp
 * \name        TLE9563-pal-wiced.cpp - Cypress Hardware Abstraction Layer
 * \author      Infineon Technologies AG
 * \copyright   2019 Infineon Technologies AG
 * \version     2.0.0
 * \ref         wicedPal
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "TLE9563-pal-wiced.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

//SPI address commands
#define TLE9563_CMD_WRITE          0x80;
#define TLE9563_CMD_CLEAR          0x80;

#define TLE9563_STATUS_INV_MASK    (Tle9563::TLE_POWER_ON_RESET)
#define TLE9563_CS_RISETIME        2

void Tle9563::writeReg(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t data)
{
	uint8_t address = mCtrlRegAddresses[reg];
	uint8_t toWrite = mCtrlRegData[reg] & (~mask);
	uint8_t byte0;
	uint8_t byte1;

	toWrite |= (data << shift) & mask;
	mCtrlRegData[reg] = toWrite;

	address = address | TLE9563_CMD_WRITE
	cs->disable();
	sBus->transfer(address,byte0);
	sBus->transfer(toWrite,byte1);
	cs->enable();
	timer->delayMilli(TLE9563_CS_RISETIME);
}

uint8_t Tle9563::readStatusReg(uint8_t reg)
{
	//read the whole register
	return readStatusReg(reg, 0xFF, 0);
}

uint8_t Tle9563::readStatusReg(uint8_t reg, uint8_t mask, uint8_t shift)
{
	uint8_t address = mStatusRegAddresses[reg];
	uint8_t byte0;
	uint8_t received;

	cs->disable();
	sBus->transfer(address,byte0);
	sBus->transfer(0xFF,received);
	cs->enable();
	timer->delayMilli(TLE9563_CS_RISETIME);

	received = (received & mask) >> shift;

	return received;
}

void Tle9563::clearStatusReg(uint8_t reg)
{
	uint8_t address = mStatusRegAddresses[reg];
	uint8_t byte0;
	uint8_t byte1;

	address = address | TLE9563_CMD_CLEAR;
	cs->disable();
	sBus->transfer(address,byte0);
	sBus->transfer(0,byte1);
	cs->enable();
	timer->delayMilli(TLE9563_CS_RISETIME);
}

#endif /** TLE9563_FRAMEWORK **/
