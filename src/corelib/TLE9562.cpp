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

void Tle9562::config(uint8_t agc = 0)
{
	_agc_status = agc;

	SBC_CRC_Disable();						// Disable cyclic redundancy check

	set_HB_ICHG_MAX(0, 3);					// Disable pulldown for off-state diagnostic and set maximum pre(dis)charge current to 100mA

	setGenControl(PWM1_TO_HB1, PWM3_TO_HB3);// Configure General bridge control reg with start configuration. 

	set_LS_and_HS_VDS(CONF_LS_AND_HS_X_VDSTH, CONF_DEEP_ADAP, CONF_TFVDS);

	for(uint8_t i=0; i<8; i++)
	{
		set_CCP_BLK(CONF_TBLANK, CONF_TCCP, i);
	}

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
	writeReg(REG_ADDR_PWM_CTRL, PWM_BNK_MODULE_4|((hss4<<4)&PWM_CTRL_DC_MASK) );    	// set dutycycle for HSS 4
  	writeReg(REG_ADDR_HS_CTRL, 0x7654);    												// assign HSS 1 to PWM1, HSS2 to PWM 2, HSS3 to PWM3
}

void Tle9562::setGenControl(bool MapPWM1, bool MapPWM3)
{
	uint16_t ToSend = (CONF_BDFREQ<<15)|(MapPWM3<<14)|(MapPWM1<<13)|(CONF_CPUVTH<<12)|(CONF_FET_LVL<<11)|(CONF_CPSTGA<<10)|(CONF_BDOV_REC<<9)|(CONF_IPCHGADT<<8)|(_agc_status<<6)|(CONF_CPEN<<5)|(CONF_POCHGDIS<<4)|(CONF_AGCFILT<<3)|(CONF_EN_GEN_CHECK<<2)|(CONF_IHOLD<<1)|(CONF_FMODE<<0);
	writeReg(REG_ADDR_GENCTRL, ToSend);
}