/*!
 * \file        TLE9563.cpp
 * \name        TLE9563.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This file has to be included in projects that use Infineon's DC Motor Control Shield with TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "TLE9563.hpp"

Tle9563::Tle9563(void)
{
	intn = NULL;
	cso = NULL;
	pwmU = NULL;
	pwmV = NULL;
	pwmU = NULL;
	bemfU = NULL;
	bemfV = NULL;
	bemfW = NULL;
	hallA = NULL;
	hallB = NULL;
	hallC = NULL;
	sBus = NULL;
	csn = NULL;
	timer = NULL;

	ActiveGround.HBmode = 0b01;		//Hier der Richtige Platz?
	ActiveGround.Freewheeling = 0;
	ActiveGround.PWMenable = 0;

	ActivePWM.HBmode = 0b10;
	ActivePWM.Freewheeling = 1;
	ActivePWM.PWMenable = 1;

	Floating.HBmode = 0b11;
	Floating.Freewheeling = 0;
	Floating.PWMenable = 0;
}

Tle9563::~Tle9563()
{
	intn = NULL;
	cso = NULL;
	pwmU = NULL;
	pwmV = NULL;
	pwmU = NULL;
	bemfU = NULL;
	bemfV = NULL;
	bemfW = NULL;
	hallA = NULL;
	hallB = NULL;
	hallC = NULL;
	sBus = NULL;
	csn = NULL;
	timer = NULL;
}

void Tle9563::config(void)
{
	SBC_CRC_Disable();
	SBC_SPI(REG_ADDR_GENCTRL, 0b1000111000110000);	//0x8E30
	SBC_SPI(REG_ADDR_LS_VDS, 0b0000000110110110);
	SBC_SPI(REG_ADDR_HS_VDS, 0b0000000110110110);
	SBC_SPI(REG_ADDR_HBMODE, 0b0000101110111011);
	SBC_SPI(REG_ADDR_SUP_STAT, 0);					//clear stat regs
}

void Tle9563::setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3)
{
	uint16_t ToSend = (hb1.HBmode<<2)|(hb1.Freewheeling<<1)|(hb1.PWMenable<<0) ;
	ToSend = ToSend | (hb2.HBmode<<6)|(hb2.Freewheeling<<5)|(hb2.PWMenable<<4);
	ToSend = ToSend | (hb3.HBmode<<10)|(hb3.Freewheeling<<9)|(hb3.PWMenable<<8);

	SBC_SPI(REG_ADDR_HBMODE, ToSend);
}