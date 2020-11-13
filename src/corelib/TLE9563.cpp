/*!
 * \file        TLE9563.cpp
 * \name        TLE9563.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to communicate with a TLE9563 BLDC controller
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

Tle9563::~Tle9563()
{
	intn = NULL;
	cso = NULL;

	sBus = NULL;
	csn = NULL;
	timer = NULL;
}

void Tle9563::config(void)
{
	SBC_CRC_Disable();

	/**
	 * 15 Bridge driver synchronization frequency: 	37Mhz
	 * 12 Charge pump under voltage:				TH1
	 * 11 External MOSFET logic level:				normal level MOSFET
	 * 10 CPSTGA:									ACTIVE
	 *  9 BDOV_REC:									Recover activated
	 *  8 IPCHGADT:									1STEP
	 *  7:6 Adaptive gate control					INACTIVE1
	 * 	5 CPEN:										Charge pump enabled
	 *  4 Postcharge phase during PWM				disabled
	 *  3 Filter for adaptive gate control			NO_FILT
	 *  2 Detection of active / FW MOSFET			disabled
	 *  1 Gate driver hold current					TH1
	 *  0 Frequency modulation of charge pump		no modulation
	 */
	//							 FEDCBA9876543210
	writeReg(REG_ADDR_GENCTRL, 0b1000111000110000);

	/**
	 * 13:12 Filter time 							500ns
	 * 8:6 LS3 drain source overvoltage threshold	800mV
	 * 5:3 LS2 drain source overvoltage threshold	800mV
	 * 2:0 LS1 drain source overvoltage threshold	800mV
	 */
	writeReg(REG_ADDR_LS_VDS, 0b0000000110110110);

	/**
	 * 12 Deep adaption enabled						NO_DEEP_ADAP
	 * 8:6 HS3 drain source overvoltage threshold	800mV
	 * 5:3 HS2 drain source overvoltage threshold	800mV
	 * 2:0 HS1 drain source overvoltage threshold	800mV
	 */
	writeReg(REG_ADDR_HS_VDS, 0b0000000110110110);

	/**
	 * 8 Periodical INTN generation					disabled
	 * 7 Disable Watchdog in SW Dev mode			disabled
	 * 6 Watchdog failure in SW Dev mode			disabled
	 * 5 SPI and CRC interrupt generation			disabled
	 * 4 Bridge Driver Interrupt generation			enabled
	 * 3 High side interrupt generation				enabled
	 * 2 BUS interrupt generation					enabled
	 * 1 Temperature Interrupt generation			enabled
	 * 0 Supply status interrupt generation			enabled
	 */
	//							  FEDCBA9876543210
	writeReg(REG_ADDR_INT_MASK, 0b0000000010010011);

	/**
	 * 10 Selection of 3 or 6 PWM inputs			3 PWM
	 * 9 Capacitance connected to CSA out			400pF
	 * 8 Direction of CSA							Bi
	 * 7:6 Overcurrent filter time of cso			6us
	 * 5 CSA OFF									CSA disabled
	 * 4:3 Overcurent detection threshold			TH1
	 * 2:1 Gain of CSA								10VV
	 * 0 Overcurrent shutdown						Disabled
	 */
	//						 FEDCBA9876543210
	writeReg(REG_ADDR_CSA, 0b0000000100100000);

	/**
	 * 
	 * 
	 */
	//							  	 FEDCBA9876543210
	writeReg(REG_ADDR_HB_ICHG_MAX, 0b0000000000010101);

	//writeReg(REG_ADDR_WD_CTRL, 0b0000000000000011);
	writeReg(REG_ADDR_SWK_CTRL, 0);

	writeReg(REG_ADDR_SUP_STAT, 0);					//clear stat regs
}

void Tle9563::begin(void)
{
	intn->init();
	cso->init();
	
	csn->init();
	csn->enable();
	sBus->init();

	timer->init();
}

void Tle9563::end(void)
{
	intn->deinit();
	cso->deinit();

	csn->deinit();
	sBus->deinit();
	
	timer->stop();
}

void Tle9563::SBC_CRC_Disable()
{
	uint8_t byte0;
	csn->disable();
	sBus->transfer(0xe7, byte0);	// 0b 1110 0111
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0x55, byte0);	// 0b 0101 0101
	sBus->transfer(0xc3, byte0);	// 0b 1100 0011
	csn->enable();
}

void Tle9563::writeReg(uint8_t addr, uint16_t data)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9563_CMD_WRITE), byte0);	// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer((data&0xFF), byte1);					// lower 8 bit of data
	sBus->transfer(((data>>8)&0xFF), byte2);			// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
}

uint16_t Tle9563::readReg(uint8_t addr)
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

void Tle9563::setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3)
{
	uint16_t ToSend = (hb1.HBmode<<2)|(hb1.Freewheeling<<1)|(hb1.PWMenable<<0);
	ToSend = ToSend | (hb2.HBmode<<6)|(hb2.Freewheeling<<5)|(hb2.PWMenable<<4);
	ToSend = ToSend | (hb3.HBmode<<10)|(hb3.Freewheeling<<9)|(hb3.PWMenable<<8);

	writeReg(REG_ADDR_HBMODE, ToSend);
}

void Tle9563::setHSS(uint16_t hss1, uint16_t hss2, uint16_t hss3)
{
	writeReg(REG_ADDR_HS_CTRL, 0);    													// disable all high side switches
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_1|((hss1<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 1
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_2|((hss2<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 2
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_3|((hss3<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 3
  	writeReg(REG_ADDR_HS_CTRL, 0x0654);    												// assign HSS 1 to PWM1, HSS2 to PWM 2, HSS3 to PWM3
}

void Tle9563::updateStatus(uint16_t *array)
{
	array[0] = readReg(REG_ADDR_SUP_STAT);
	writeReg(REG_ADDR_SUP_STAT, 0);
	array[1] = readReg(REG_ADDR_THERM_STAT);
	writeReg(REG_ADDR_THERM_STAT, 0);
	
}
