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

uint8_t Tle9xxx::writeReg(uint8_t addr, uint16_t data)
{
	uint8_t byte0, byte1, byte2, byte3;
	csn->disable();
	sBus->transfer((addr|TLE9xxx_CMD_WRITE), byte0);	// MSB: Read/write cmd, bit 6:0 register address
	sBus->transfer((data&0xFF), byte1);					// lower 8 bit of data
	sBus->transfer(((data>>8)&0xFF), byte2);			// upper 8 bit of data
	sBus->transfer(0xA5, byte3);						// for CRC disabled, fill with static pattern
	csn->enable();
	return byte0;
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

uint8_t Tle9xxx::checkStatusInformationField(void)
{
	uint8_t ErrorCode = 0;
    uint16_t RegAddress = 0;
    uint16_t RegContent = 0;
    ErrorCode = checkStatSUP(RegAddress, RegContent);		// Read the first register at the beginning in order to read the actual SIF values.
	if(_statusInformationField & SIF_SUPPLY_STAT) PrintTLEErrorMessage(ErrorCode, RegAddress, RegContent);
	if(_statusInformationField & SIF_TEMP_STAT) PrintTLEErrorMessage(checkStatTHERM(RegAddress, RegContent), RegAddress, RegContent);
	if(_statusInformationField & SIF_BUS_STAT) ;
	if(_statusInformationField & SIF_WAKE_UP) ;
	if(_statusInformationField & SIF_HS_STAT) PrintTLEErrorMessage(checkStatHSS(RegAddress, RegContent), RegAddress, RegContent);
	if(_statusInformationField & SIF_DEV_STAT) PrintTLEErrorMessage(checkStatDEV(RegAddress, RegContent), RegAddress, RegContent);
	if(_statusInformationField & SIF_BD_STAT) PrintTLEErrorMessage(checkStatDSOV(RegAddress, RegContent), RegAddress, RegContent);
	if(_statusInformationField & SIF_SPI_CRC_FAIL) ;

    return _statusInformationField;
}


uint8_t Tle9xxx::checkStatSUP(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_SUP_STAT);
	_statusInformationField = writeReg(REG_ADDR_SUP_STAT, 0);
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

uint8_t Tle9xxx::checkStatDSOV(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint8_t ErrorCode = 0;
	input = readReg(REG_ADDR_DSOV);
	writeReg(REG_ADDR_DSOV, 0);
	RegAddress = REG_ADDR_DSOV;
	RegContent = input;
	if((input | 0x00FF) > 0){
		Serial.println("==> HS or VS overvoltage error! <==");
		ErrorCode = TLE_SHORT_CIRCUIT;
	}

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
        Serial.print("\tReg: 0x");
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

void Tle9xxx::set_ST_ICHG(uint8_t ICHGST1, uint8_t ICHGST2, uint8_t ICHGST3, uint8_t ICHGST4)
{
	uint16_t ToSend  = ((ICHGST4<<12) & 0xF000) | ((ICHGST3<<8) & 0x0F00) | ((ICHGST2<<4) & 0x00F0) | (ICHGST1 & 0x000F);
	writeReg(REG_ADDR_ST_ICHG, ToSend);
}

void Tle9xxx::set_HB_ICHG(uint8_t IDCHG, uint8_t ICHG, bool ACTorFW, uint8_t hb)
{
	uint16_t ToSend  = ((IDCHG<<10) & 0xFC00) | ((ICHG<<4) & 0x03F0) | (((ACTorFW<<2)|hb)& 0x000F);
	writeReg(REG_ADDR_HB_ICHG, ToSend);
}

void Tle9xxx::set_HB_ICHG_MAX(uint8_t HBxIDIAG, uint8_t ICHGMAXx)
{
	uint16_t ToSend = 0;
	for(uint8_t i=0; i < 4; i++)
	{
		ToSend = ToSend | ((HBxIDIAG<<(i+12)) |  (HBxIDIAG<<(i*2)));
	}

	writeReg(REG_ADDR_HB_ICHG_MAX, ToSend);
}

void Tle9xxx::set_PCHG_INIT(uint8_t PDCHGINIT, uint8_t PCHGINIT, uint8_t INIT_BNK)
{
	
}

void Tle9xxx::set_TDON_HB_CTRL(uint8_t TDON, uint8_t HB_TDON_BNK)
{

}

void Tle9xxx::set_TDOFF_HB_CTRL(uint8_t TDOFF, uint8_t HB_TDOFF_BNK)
{

}

void Tle9xxx::set_LS_and_HS_VDS(uint8_t VDSTH, bool DEEP_ADAP, uint8_t TFVDS)
{
	uint16_t ToSend = ((TFVDS<<12) & 0x3000) | ((VDSTH<<9) & 0xE00) | ((VDSTH<<6) & 0x1C0) | ((VDSTH<<3) & 0x38) | (VDSTH & 0x7);
	writeReg(REG_ADDR_LS_VDS, ToSend);

	ToSend = 0;
	ToSend = ((DEEP_ADAP<<12) & 0x1000) | ((VDSTH<<9) & 0xE00) | ((VDSTH<<6) & 0x1C0) | ((VDSTH<<3) & 0x38) | (VDSTH & 0x7);

	writeReg(REG_ADDR_HS_VDS, ToSend);
}

void Tle9xxx::set_CCP_BLK(uint8_t TBLANK, uint8_t TCCP, uint8_t CCP_BNK)
{
	uint16_t ToSend = ((TBLANK<<12)&0xF000) | ((TCCP<<8)&0xF00) | (CCP_BNK & 0x7);
	writeReg(REG_ADDR_CCP_BLK, ToSend);
}


void Tle9xxx::checkStat_TRISE_FALL(uint8_t hb, uint8_t &Trise, uint8_t &Tfall)
{
	uint16_t input=0;
	uint16_t reg=0;
	switch(hb){
		case HB1: reg = REG_ADDR_TRISE_FALL1; break;
		case HB2: reg = REG_ADDR_TRISE_FALL2; break;
		case HB3: reg = REG_ADDR_TRISE_FALL3; break;
		case HB4: reg = REG_ADDR_TRISE_FALL4; break;
	}

	input = readReg(reg);
    Trise = input & 0x3F;
	Tfall = (input>>8) & 0x3F;
	//Trise = 53.3 * t_rise;					// [ns]
	//Tfall = 53.3 * t_fall;					// [ns]
}

void Tle9xxx::init_AGC_Algorithm(uint8_t hb)
{
    // Initialize the algorithm variable
    m_ichg = CONF_INIT_ICHG;
    // Initialize the ICHG at the TLE9562
	set_HB_ICHG(m_idchg, CONF_INIT_ICHG, 0, hb);

    // The elements with an index lower than (CONF_INIT_ICHG - MIN_ICHG) are initialized
    // with a 0, the rest of the elements with EOS
    for (int i = 0; i < (CONF_INIT_ICHG - MIN_ICHG); i++) m_trise_ema[i] = 0;
    for (int i = (CONF_INIT_ICHG - MIN_ICHG); i <= (MAX_ICHG - MIN_ICHG); i++)  m_trise_ema[i] = EOS;
}

void Tle9xxx::emaCalculation(uint8_t hb, uint8_t &risetime, uint8_t &falltime)
{
    uint8_t tRISE, tFALL;
    // Read the last tRISE measured by the TLE9562
    //tRISE = readSpitRISE();
	checkStat_TRISE_FALL(hb, tRISE, tFALL);

    // If m_ichg still has the initialization value, tRISEx is directly m_trisex_ema
    // without calculating the EMA
    if (m_trise_ema[m_ichg - MIN_ICHG] == EOS || m_trise_ema[m_ichg - MIN_ICHG] == 0)
        m_trise_ema[m_ichg - MIN_ICHG] = tRISE * SCALING_FACTOR_FPA;
    else
        // EMA calculated and stored in m_trise_ema for the current charge current
        m_trise_ema[m_ichg - MIN_ICHG] = tRISE * ALPHA3_FPA_SCALED + (m_trise_ema[m_ichg - MIN_ICHG] / SCALING_FACTOR_FPA) * ONE_MINUS_ALPHA3_FPA_SCALED ;
	risetime = tRISE;
	falltime = tFALL;
}

void Tle9xxx::adaptiveHysteresisDecisionTree (uint8_t hb)
{
    // The charge current is decreased if all conditions are true:
    // 1. The EMA calculated for the current ICHG3 is lower than CONF_TRISE_TG
    // 2. The EMA that was calculated when the ICHG3 was set to the
    // immediately lower value is also lower than tRISEx_TG
    // 3. The minimum ICHGx has still not been reached.
    if ((m_trise_ema[m_ichg - MIN_ICHG] <= (CONF_TRISE_TG * SCALING_FACTOR_FPA))
        && (m_trise_ema[m_ichg - MIN_ICHG - 1] < (CONF_TRISE_TG * SCALING_FACTOR_FPA))
        && m_ichg > MIN_ICHG)
        m_ichg --;

    // The charge current is increased if these three conditions are met:
    // 1. The EMA calculated for the current ICHGx is higher than CONF_TRISE_TG
    // 2. The EMA that was calculated when the ICHGx was set to the
    // immediately higher value of the current one is also higher than CONF_TRISE_TG
    // 3. The maximum ICHGx has still not been reached.
    if ((m_trise_ema[m_ichg - MIN_ICHG] >= (CONF_TRISE_TG * SCALING_FACTOR_FPA))
        && (m_trise_ema[m_ichg - MIN_ICHG + 1] > (CONF_TRISE_TG * SCALING_FACTOR_FPA))
        && m_ichg < MAX_ICHG)
        m_ichg ++;

    // Set the ICHG at the TLE9562
	set_HB_ICHG(m_idchg, m_ichg, 0, hb);
}