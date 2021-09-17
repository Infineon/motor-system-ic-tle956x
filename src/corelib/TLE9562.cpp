/*!
 * \file        TLE9562.cpp
 * \name        TLE9562.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This library includes the basic common functions to communicate with a TLE9562 motor controller
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include "TLE9562.hpp"

Tle9562::Tle9562(void)
{

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

Tle9562::~Tle9562()
{
	
}

void Tle9562::config(void)
{
	/**
	 * TODO: split up in separate functions for better control from outside the library
	 * 
	 */

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

void Tle9562::begin(void)
{
	intn->init();
	
	csn->init();
	csn->enable();
	sBus->init();

	timer->init();
}

void Tle9562::end(void)
{
	intn->deinit();

	csn->deinit();
	sBus->deinit();
	
	timer->stop();
}

void Tle9562::setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3, HBconfig_t hb4)
{
	uint16_t ToSend = (hb1.HBmode<<2)|(hb1.Freewheeling<<1)|(hb1.PWMenable<<0);
	ToSend = ToSend | (hb2.HBmode<<6)|(hb2.Freewheeling<<5)|(hb2.PWMenable<<4);
	ToSend = ToSend | (hb3.HBmode<<10)|(hb3.Freewheeling<<9)|(hb3.PWMenable<<8);
    ToSend = ToSend | (hb4.HBmode<<14)|(hb4.Freewheeling<<13)|(hb4.PWMenable<<12);

	writeReg(REG_ADDR_HBMODE, ToSend);
}

void Tle9562::setHSS(uint16_t hss1, uint16_t hss2, uint16_t hss3, uint16_t hss4)
{
	writeReg(REG_ADDR_HS_CTRL, 0);    													// disable all high side switches
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_1|((hss1<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 1
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_2|((hss2<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 2
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_3|((hss3<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 3
  	writeReg(REG_ADDR_HS_CTRL, 0x0654);    												// assign HSS 1 to PWM1, HSS2 to PWM 2, HSS3 to PWM3
}
