/*!
 * @file        TLE9562.cpp
 * @name        TLE9562.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to communicate with a TLE9562 motor controller
 * @ref         tle9563corelib
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

void Tle9562::config(uint8_t agc)
{
	_agc_status = agc;													// set the global variable, if agc is used, as this is used very often in the GENctrl register

	SBC_CRC_Disable();													// Disable cyclic redundancy check

	for(uint8_t i = PHASE1; i <= PHASE4; i++)
	{
		set_HB_ICHG(m_idchg, m_ichg, ACTIVE_MOSFET, i);					// set static charge and discharge current for active MOSFETs
		set_HB_ICHG(0, CONF_ICHG_FW, FW_MOSFET, i);						// set static charge and discharge currents for freewheeling MOSFETs

		set_CCP_BLK(CONF_TBLANK_ACT, CONF_TCCP_ACT, ACTIVE_MOSFET, i);	// set the blank time and cross current protection time for active MOSFETS
		set_CCP_BLK(CONF_TBLANK_FW, CONF_TCCP_FW, FW_MOSFET, i);		// set the blank time and cross current protection time for free wheeling MOSFETS

		set_PCHG_INIT(CONF_PDCHG_INIT, CONF_PCHG_INIT, i);				// set initial precharge and pre-discharge current for internal AGC
		set_TDON_HB_CTRL(CONF_TDOFF, i);								// set desired turn-off delay
		set_TDOFF_HB_CTRL(CONF_TDON, i);								// set desired turn-on delay
	}
	
	set_TPRECHG(CONF_TPCHGX, CONF_TPDCHGX);								// set precharge and predischarge time
	set_HB_ICHG_MAX(CONF_HBXIDIAG, CONF_ICHGMAXX);						// Disable pulldown for off-state diagnostic and set maximum pre(dis)charge current to 100mA
	set_ST_ICHG(CONF_ICHGST, CONF_ICHGST, CONF_ICHGST, CONF_ICHGST);	// set static charge and discharge currents
	setGenControl(PWM1_TO_HB1, PWM3_TO_HB3);							// Configure General bridge control reg with start configuration. 
	set_LS_and_HS_VDS(CONF_LS_AND_HS_X_VDSTH, CONF_DEEP_ADAP, CONF_TFVDS);

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