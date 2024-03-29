/*!
 * @file        TLE9xxx.cpp
 * @name        TLE9xxx.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9xxx
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the generic functions for TLE9xxx family
 * @ref         tle9563corelib
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

Tle9xxx::~Tle9xxx(void)
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
	timer->delayMicro(TLE956x_CS_RISETIME);
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
	timer->delayMicro(TLE956x_CS_RISETIME);
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
	timer->delayMicro(TLE956x_CS_RISETIME);
	return ((byte2<<8)|byte1);
}

void Tle9xxx::configInterruptMask(void)
{
	uint16_t tosend = 	WD_SDM_DISABLE | BD_STAT | HS_STAT | TEMP_STAT | SUPPLY_STAT;
						
	writeReg(REG_ADDR_INT_MASK, tosend);
}

uint16_t Tle9xxx::checkStatusInformationField(void)
{
	uint16_t ErrorCode = 0;
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

    return _error_enable;
}


uint16_t Tle9xxx::checkStatSUP(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint16_t ErrorCode = 0;
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

uint16_t Tle9xxx::checkStatTHERM(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint16_t ErrorCode = 0;
	input = readReg(REG_ADDR_THERM_STAT);
	writeReg(REG_ADDR_THERM_STAT, 0);
	RegAddress = REG_ADDR_THERM_STAT;
	RegContent = input;
	if((input & 0x000E) > 0) ErrorCode = TLE_TEMP_SHUTDOWN; 			// overtemperature detection
	else if((input & 0x0001) > 0) ErrorCode |= TLE_TEMP_SHUTDOWN;		// temperature warning

	return ErrorCode;
}

uint16_t Tle9xxx::checkStatHSS(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint16_t ErrorCode = 0;
	input = readReg(REG_ADDR_HS_OL_OC_OT_STAT);
	writeReg(REG_ADDR_HS_OL_OC_OT_STAT, 0);
	RegAddress = REG_ADDR_HS_OL_OC_OT_STAT;
	RegContent = input;
	if((input & 0x1C00) > 0) ErrorCode = TLE_TEMP_SHUTDOWN; 		// overtemperature detection
	else if((input & 0x00E0) > 0) ErrorCode |= TLE_LOAD_ERROR;		// open load detection
	else if((input & 0x0007) > 0) ErrorCode |= TLE_OVERCURRENT;		// overcurrent detection

	return ErrorCode;
}

uint16_t Tle9xxx::checkStatDEV(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint16_t ErrorCode = 0;
	input = readReg(REG_ADDR_DEV_STAT);
	writeReg(REG_ADDR_DEV_STAT, 0);
	RegAddress = REG_ADDR_DEV_STAT;
	RegContent = input;
	if((input & 0x0003) > 0) ErrorCode = TLE_SPI_ERROR; 			// SPI Error

	return ErrorCode;
}

uint16_t Tle9xxx::checkStatDSOV(uint16_t &RegAddress, uint16_t &RegContent)
{
	uint16_t input=0;
	uint16_t ErrorCode = 0;
	input = readReg(REG_ADDR_DSOV);
	writeReg(REG_ADDR_DSOV, 0);
	RegAddress = REG_ADDR_DSOV;
	RegContent = input;
	if((input & 0x00FF) > 0) ErrorCode = TLE_HS_LS_OVERVOLTAGE;
	if((input & 0x4000) > 0) ErrorCode |= TLE_OC_CSA_OVERCURRENT;

	return ErrorCode;
}

bool Tle9xxx::PrintTLEErrorMessage(uint16_t msg, uint16_t &RegAddress, uint16_t &RegContent)
{
    if(msg & TLE_SPI_ERROR)
    {
        Serial.println(F("===> Error: SPI-Failure <==="));
    }
    if(msg & TLE_LOAD_ERROR)
    {
        Serial.println(F("===> Error: Open-Load detected! <==="));
    }
    if(msg & TLE_UNDER_VOLTAGE)
    {
        Serial.println(F("===> Error: Undervoltage detected! Check your voltage supply <==="));
    }
    if(msg & TLE_OVER_VOLTAGE)
    {
        Serial.println(F("===> Error: Overvoltage detected! Check your voltage supply <===="));
    }
    if(msg & TLE_POWER_ON_RESET)
    {
        Serial.println(F("===> Power on reset detected! <==="));
    }
    if(msg & TLE_TEMP_SHUTDOWN)
    {
        Serial.println(F("===> Error: Temperature shutdown <==="));
    }
    if(msg & TLE_OVERCURRENT)
    {
        Serial.println(F("===> Error: Overcurrent detected! <==="));
    }
    if(msg & TLE_SHORT_CIRCUIT)
    {
        Serial.println(F("===> Error: Short circuit detected! <==="));
    }
	if(msg & TLE_HS_LS_OVERVOLTAGE)
    {
		Serial.println(F("===> Error: HS / LS Overvoltage <==="));
	}
	if(msg & TLE_OC_CSA_OVERCURRENT)
	{
		Serial.println(F("===> Error: CSA Overcurrent <==="));
	}

    if(msg > 0)
    {
        Serial.print(F("\tReg: 0x"));
        Serial.print(RegAddress, HEX);
        Serial.print(F("  Content: 0x"));
		Serial.print(RegContent, HEX);
        Serial.println("");
		_error_enable = 1;
    }
}

void Tle9xxx::set_TPRECHG(uint8_t t_pchgx, uint8_t t_pdchgx)
{
	uint16_t ToSend = 0;
	ToSend = ((t_pchgx << 13) & 0xE000) | ((t_pchgx << 10) & 0x1C00)| ((t_pchgx << 7) & 0x380)| ((t_pchgx << 4) & 0x70);
	writeReg(REG_ADDR_TPRECHG, ToSend);				// Set Precharge time

	ToSend = ((t_pdchgx << 13) & 0xE000) | ((t_pdchgx << 10) & 0x1C00)| ((t_pdchgx << 7) & 0x380)| ((t_pdchgx << 4) & 0x70) | (0x001);
	writeReg(REG_ADDR_TPRECHG, ToSend);				// Set Pre-Discharge time
}


void Tle9xxx::set_ST_ICHG(uint8_t ichgst1, uint8_t ichgst2, uint8_t ichgst3, uint8_t ichgst4)
{
	uint16_t ToSend  = ((ichgst4<<12) & 0xF000) | ((ichgst3<<8) & 0x0F00) | ((ichgst2<<4) & 0x00F0) | (ichgst1 & 0x000F);
	writeReg(REG_ADDR_ST_ICHG, ToSend);
}

void Tle9xxx::set_HB_ICHG(uint8_t idchg, uint8_t ichg, bool ACTorFW, uint8_t hb)
{
	uint16_t ToSend  = ((idchg<<10) & 0xFC00) | ((ichg<<4) & 0x03F0) | (((ACTorFW<<2)|hb)& 0x000F);
	writeReg(REG_ADDR_HB_ICHG, ToSend);
}

void Tle9xxx::set_HB_ICHG_MAX(uint8_t hbxidiag, uint8_t ichgMaxx)
{
	uint16_t ToSend = 0;
	for(uint8_t i=PHASE1; i < PHASE4; i++)
	{
		ToSend = ToSend | ((hbxidiag<<(i+12)) |  (ichgMaxx<<(i*2)));
	}

	writeReg(REG_ADDR_HB_ICHG_MAX, ToSend);
}

void Tle9xxx::set_PCHG_INIT(uint8_t pdchg_init, uint8_t pchg_init, uint8_t init_bnk)
{
	uint16_t ToSend  = ((pdchg_init<<10) & 0xFC00) | ((pchg_init<<4) & 0x03F0) | (init_bnk& 0x7);
	writeReg(REG_ADDR_HB_PCHG_INIT, ToSend);
}

void Tle9xxx::set_TDON_HB_CTRL(uint8_t t_don, uint8_t hb_tdon_bnk)
{
	uint16_t ToSend  = ((t_don<<8) & 0x3F00) | (hb_tdon_bnk & 0x7);
	writeReg(REG_ADDR_TDON_HB_CTRL, ToSend);
}

void Tle9xxx::set_TDOFF_HB_CTRL(uint8_t t_doff, uint8_t hb_tdoff_bnk)
{
	uint16_t ToSend  = ((t_doff<<8) & 0x3F00) | (hb_tdoff_bnk & 0x7);
	writeReg(REG_ADDR_TDOFF_HB_CTRL, ToSend);
}

void Tle9xxx::set_LS_and_HS_VDS(uint8_t vdsth, bool deep_adap, uint8_t tfvds)
{
	uint16_t ToSend = ((tfvds<<12) & 0x3000) | ((vdsth<<9) & 0xE00) | ((vdsth<<6) & 0x1C0) | ((vdsth<<3) & 0x38) | (vdsth & 0x7);
	writeReg(REG_ADDR_LS_VDS, ToSend);

	ToSend = 0;
	ToSend = ((deep_adap<<12) & 0x1000) | ((vdsth<<9) & 0xE00) | ((vdsth<<6) & 0x1C0) | ((vdsth<<3) & 0x38) | (vdsth & 0x7);

	writeReg(REG_ADDR_HS_VDS, ToSend);
}

void Tle9xxx::set_CCP_BLK(uint8_t t_blank, uint8_t t_ccp, bool ACTorFW, uint8_t hb)
{
	uint16_t ToSend = ((t_blank<<12)&0xF000) | ((t_ccp<<8)&0xF00) | (((ACTorFW<<2)|hb)& 0x000F);
	writeReg(REG_ADDR_CCP_BLK, ToSend);
}


void Tle9xxx::checkStat_TRISE_FALL(uint8_t hb, uint8_t &Trise, uint8_t &Tfall)
{
	uint16_t input=0;
	uint16_t reg=0;
	switch(hb){
		case PHASE1: reg = REG_ADDR_TRISE_FALL1; break;
		case PHASE2: reg = REG_ADDR_TRISE_FALL2; break;
		case PHASE3: reg = REG_ADDR_TRISE_FALL3; break;
		case PHASE4: reg = REG_ADDR_TRISE_FALL4; break;
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
	m_idchg = CONF_INIT_IDCHG;
    // Initialize the ICHG at the TLE9562
	set_HB_ICHG(CONF_INIT_IDCHG, CONF_INIT_ICHG, ACTIVE_MOSFET, hb);

    // The elements with an index lower than (CONF_INIT_ICHG - MIN_ICHG) are initialized
    // with a 0, the rest of the elements with EOS
    for (int i = 0; i < (CONF_INIT_ICHG - MIN_ICHG); i++) m_trise_ema[i] = 0;
    for (int i = (CONF_INIT_ICHG - MIN_ICHG); i <= (MAX_ICHG - MIN_ICHG); i++)  m_trise_ema[i] = EOS;

	for (int i = 0; i < (CONF_INIT_IDCHG - MIN_ICHG); i++) m_tfall_ema[i] = 0;
    for (int i = (CONF_INIT_IDCHG - MIN_ICHG); i <= (MAX_ICHG - MIN_ICHG); i++)  m_tfall_ema[i] = EOS;
}

void Tle9xxx::emaCalculation(uint8_t hb, uint8_t * risetime, uint8_t * falltime)
{
    uint8_t tRISE, tFALL;
    // Read the last tRISE measured by the TLE9562
    //tRISE = readSpitRISE();
	checkStat_TRISE_FALL(hb, tRISE, tFALL);

	/******* Risetime ********/
    // If m_ichg still has the initialization value, tRISEx is directly m_trisex_ema
    // without calculating the EMA
    if (m_trise_ema[m_ichg - MIN_ICHG] == EOS || m_trise_ema[m_ichg - MIN_ICHG] == 0)
        m_trise_ema[m_ichg - MIN_ICHG] = tRISE * SCALING_FACTOR_FPA;
    else
        // EMA calculated and stored in m_trise_ema for the current charge current
        m_trise_ema[m_ichg - MIN_ICHG] = tRISE * ALPHA3_FPA_SCALED + (m_trise_ema[m_ichg - MIN_ICHG] / SCALING_FACTOR_FPA) * ONE_MINUS_ALPHA3_FPA_SCALED ;

	/******* Falltime ********/
	if (m_tfall_ema[m_idchg - MIN_ICHG] == EOS || m_tfall_ema[m_idchg - MIN_ICHG] == 0)
        m_tfall_ema[m_idchg - MIN_ICHG] = tFALL * SCALING_FACTOR_FPA;
    else
        // EMA calculated and stored in m_tfall_ema for the current charge current
        m_tfall_ema[m_idchg - MIN_ICHG] = tFALL * ALPHA3_FPA_SCALED + (m_tfall_ema[m_idchg - MIN_ICHG] / SCALING_FACTOR_FPA) * ONE_MINUS_ALPHA3_FPA_SCALED ;

	*risetime = tRISE;
	*falltime = tFALL;
	//Serial.print("RISEtime:");
	//Serial.println(tRISE);
}

void Tle9xxx::adaptiveHysteresisDecisionTree (uint8_t hb, uint8_t * ichg, uint8_t * idchg)
{
	/******* Risetime ********/
    // The charge current is decreased if all conditions are true:
    // 1. The EMA calculated for the current ICHG3 is lower than m_trise_tg
    // 2. The EMA that was calculated when the ICHG3 was set to the
    // immediately lower value is also lower than tRISEx_TG
    // 3. The minimum ICHGx has still not been reached.
    if ((m_trise_ema[m_ichg - MIN_ICHG] <= (m_trise_tg * SCALING_FACTOR_FPA))
        && (m_trise_ema[m_ichg - MIN_ICHG - 1] < (m_trise_tg * SCALING_FACTOR_FPA))
        && m_ichg > MIN_ICHG)
        m_ichg --;

    // The charge current is increased if these three conditions are met:
    // 1. The EMA calculated for the current ICHGx is higher than m_trise_tg
    // 2. The EMA that was calculated when the ICHGx was set to the
    // immediately higher value of the current one is also higher than m_trise_tg
    // 3. The maximum ICHGx has still not been reached.
    if ((m_trise_ema[m_ichg - MIN_ICHG] >= (m_trise_tg * SCALING_FACTOR_FPA))
        && (m_trise_ema[m_ichg - MIN_ICHG + 1] > (m_trise_tg * SCALING_FACTOR_FPA))
        && m_ichg < MAX_ICHG)
        m_ichg ++;

	/******* Falltime ********/
	if ((m_tfall_ema[m_idchg - MIN_ICHG] <= (m_tfall_tg * SCALING_FACTOR_FPA))
        && (m_tfall_ema[m_idchg - MIN_ICHG - 1] < (m_tfall_tg * SCALING_FACTOR_FPA))
        && m_idchg > MIN_ICHG)
        m_idchg --;

	if ((m_tfall_ema[m_idchg - MIN_ICHG] >= (m_tfall_tg * SCALING_FACTOR_FPA))
        && (m_tfall_ema[m_idchg - MIN_ICHG + 1] > (m_tfall_tg * SCALING_FACTOR_FPA))
        && m_idchg < MAX_ICHG)
        m_idchg ++;
	
    // Set the ICHG at the TLE9562
	set_HB_ICHG(m_idchg, m_ichg, ACTIVE_MOSFET, hb);

	*ichg = m_ichg;			// Hand over to the pointervariable
	*idchg = m_idchg;
}