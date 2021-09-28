/*!
 * \file        TLE9xxx.cpp
 * \name        TLE9xxx.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9xxx
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This library includes the generic functions for TLE9xxx family
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include "TLE9xxx.hpp"

Tle9xxx::Tle9xxx(void)
{
	intn = NULL;
	sBus = NULL;
	csn = NULL;
	timer = NULL;

	ActiveGround.HBmode = 0b01;
	ActiveGround.Freewheeling = 0;
	ActiveGround.PWMenable = 0;

	ActivePWM.HBmode = 0b10;
	ActivePWM.Freewheeling = 1;
	ActivePWM.PWMenable = 1;

	Floating.HBmode = 0b11;
	Floating.Freewheeling = 0;
	Floating.PWMenable = 0;
}

Tle9xxx::~Tle9xxx()
{
	intn = NULL;

	sBus = NULL;
	csn = NULL;
	timer = NULL;
}

void Tle9xxx::SBC_CRC_Disable()
{
	uint8_t byte0;
	csn->disable();
	sBus->transfer(0xe7, byte0);	// 0b 1110 0111
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0xc3, byte0);	// 0b 1100 0011
	csn->enable();
}

void Tle9xxx::writeReg(uint8_t addr, uint16_t data)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9xxx_CMD_WRITE), byte0);	// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer((data&0xFF), byte1);					// lower 8 bit of data
	sBus->transfer(((data>>8)&0xFF), byte2);			// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
}

uint16_t Tle9xxx::readReg(uint8_t addr)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9xxx_CMD_READ), byte0);		// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer(0xFF, byte1);						// lower 8 bit of data
	sBus->transfer(0xFF, byte2);						// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
	return ((byte2<<8)|byte1);
}

void Tle9xxx::configInterruptMask(void)
{
	uint16_t tosend = 	WD_SDM_DISABLE | BD_STAT | HS_STAT | TEMP_STAT | SUPPLY_STAT;
						
	writeReg(REG_ADDR_INT_MASK, tosend);
}

uint8_t Tle9xxx::checkStatSUP(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_SUP_STAT);
	writeReg(REG_ADDR_SUP_STAT, 0);
	RegAddress = REG_ADDR_SUP_STAT;
	RegContent = input;
	if((input & 0x1000) > 0) ErrorCode = TLE_TEMP_SHUTDOWN; 			// overtemperature detection (chargepump)
	else if((input & 0x0D55) > 0) ErrorCode |= TLE_UNDER_VOLTAGE;		// undervoltage detection
	else if((input & 0x02A2) > 0) ErrorCode |= TLE_OVER_VOLTAGE;		// overvoltage detection
	else if((input & 0x0008) > 0) ErrorCode |= TLE_SHORT_CIRCUIT;		// short circuit detection
	else if((input & 0x8000) > 0) ErrorCode |= TLE_POWER_ON_RESET;		// Power-On reset detection

	return ErrorCode;
}

uint8_t Tle9xxx::checkStatTHERM(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_THERM_STAT);
	writeReg(REG_ADDR_THERM_STAT, 0);
	RegAddress = REG_ADDR_THERM_STAT;
	RegContent = input;
	if((input & 0x000E) > 0) ErrorCode = TLE_TEMP_SHUTDOWN; 			// overtemperature detection
	else if((input & 0x0001) > 0) ErrorCode |= TLE_TEMP_SHUTDOWN;		// temperature warning

	return ErrorCode;
}

uint8_t Tle9xxx::checkStatHSS(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_HS_OL_OC_OT_STAT);
	writeReg(REG_ADDR_HS_OL_OC_OT_STAT, 0);
	RegAddress = REG_ADDR_HS_OL_OC_OT_STAT;
	RegContent = input;
	if((input & 0x1C00) > 0) ErrorCode = TLE_TEMP_SHUTDOWN; 		// overtemperature detection
	else if((input & 0x00E0) > 0) ErrorCode |= TLE_LOAD_ERROR;		// open load detection
	else if((input & 0x0007) > 0) ErrorCode |= TLE_OVERCURRENT;		// overcurrent detection

	return ErrorCode;
}

uint8_t Tle9xxx::checkStatDEV(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_DEV_STAT);
	writeReg(REG_ADDR_DEV_STAT, 0);
	RegAddress = REG_ADDR_DEV_STAT;
	RegContent = input;
	if((input & 0x0103) > 0) ErrorCode = TLE_SPI_ERROR; 			// CRC / SPI Error

	return ErrorCode;
}

bool Tle9xxx::PrintTLEErrorMessage(uint8_t msg, uint16_t &RegAddress, uint16_t &RegContent)
{
    if(msg & TLE_SPI_ERROR)
    {
        Serial.println("===> Error: CRC / SPI-Failure <===");
    }
    else if(msg & TLE_LOAD_ERROR)
    {
        Serial.println("===> Error: Open-Load detected! <===");
    }
    else if(msg & TLE_UNDER_VOLTAGE)
    {
        Serial.println("===> Error: Undervoltage detected! Check your voltage supply <===");
    }
    else if(msg & TLE_OVER_VOLTAGE)
    {
        Serial.println("===> Error: Overvoltage detected! Check your voltage supply <====");
    }
    else if(msg & TLE_POWER_ON_RESET)
    {
        Serial.println("===> Power on reset detected! <===");
    }
    else if(msg & TLE_TEMP_SHUTDOWN)
    {
        Serial.println("===> Error: Temperature shutdown <===");
    }
    else if(msg & TLE_OVERCURRENT)
    {
        Serial.println("===> Error: Overcurrent detected! <===");
    }
    else if(msg & TLE_SHORT_CIRCUIT)
    {
        Serial.println("===> Error: Short circuit detected! <===");
    }

    if(DETAILED_ERROR_REPORT)
    {
        Serial.print("Reg: 0x");
        Serial.print(RegAddress, HEX);
        Serial.print("  Content: 0x");
		Serial.print(RegContent, HEX);
        //PrintBinary(16, RegContent);
        Serial.println("");
    }
}

void Tle9xxx::PrintBinary(uint8_t digits, uint16_t number)
{
  for(uint8_t i=digits-1; i>0; i--)
  {
    if(pow(2, i) <= number)
    {
      break;
    }
    Serial.print("0");
  }
  Serial.print(number,BIN);
}