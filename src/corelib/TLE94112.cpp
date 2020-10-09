/*!
 * \file        TLE9563.cpp
 * \name        TLE9563.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This file has to be included in projects that use Infineon's DC Motor Control Shield with TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "TLE9563.hpp"

#define TLE9563_STATUS_INV_MASK    (Tle9563::TLE_POWER_ON_RESET)

Tle9563::Tle9563(void)
{
	sBus = NULL;
	en = NULL;
	cs = NULL;
	timer = NULL;
}

Tle9563::Tle9563(void* bus, uint8_t csPin)
{
	Tle9563();
}

Tle9563::~Tle9563()
{
	en = NULL;
	cs = NULL;
	timer = NULL;
	sBus = NULL;
}

void Tle9563::configHB(HalfBridge hb, HBState state, PWMChannel pwm)
{
	configHB(hb, state, pwm, 0);
}

void Tle9563::configHB(HalfBridge hb, HBState state, PWMChannel pwm, uint8_t activeFW)
{
	configHB(static_cast<uint8_t>(hb),static_cast<uint8_t>(state),static_cast<uint8_t>(pwm),activeFW );
}

void Tle9563::configHB(uint8_t hb, uint8_t state, uint8_t pwm, uint8_t activeFW)
{
	uint8_t reg = mHalfBridges[hb].stateReg;
	uint8_t mask = mHalfBridges[hb].stateMask;
	uint8_t shift = mHalfBridges[hb].stateShift;
	writeReg(reg, mask, shift, state);

	reg = mHalfBridges[hb].pwmReg;
	mask = mHalfBridges[hb].pwmMask;
	shift = mHalfBridges[hb].pwmShift;
	writeReg(reg, mask, shift, pwm);

	reg = mHalfBridges[hb].fwReg;
	mask = mHalfBridges[hb].fwMask;
	shift = mHalfBridges[hb].fwShift;
	writeReg(reg, mask, shift, activeFW);
}

void Tle9563::configPWM(PWMChannel pwm, PWMFreq freq, uint8_t dutyCycle)
{
	configPWM(static_cast<uint8_t>(pwm),static_cast<uint8_t>(freq),dutyCycle );
}

void Tle9563::configPWM(uint8_t pwm, uint8_t freq, uint8_t dutyCycle)
{
	uint8_t reg = mPwmChannels[pwm].freqReg;
	uint8_t mask = mPwmChannels[pwm].freqMask;
	uint8_t shift = mPwmChannels[pwm].freqShift;
	writeReg(reg, mask, shift, freq);

	reg = mPwmChannels[pwm].dcReg;
	mask = mPwmChannels[pwm].dcMask;
	shift = mPwmChannels[pwm].dcShift;
	writeReg(reg, mask, shift, dutyCycle);
}

uint8_t Tle9563::getSysDiagnosis()
{
	uint8_t ret = readStatusReg(SYS_DIAG1);
	return ret^TLE9563_STATUS_INV_MASK;			// $$$$$$$$$$$$$$$$$$$$$$ Bitte was?
}

uint8_t Tle9563::getSysDiagnosis(DiagFlag mask)
{
	return getSysDiagnosis(static_cast<uint8_t>(mask));
}

uint8_t Tle9563::getSysDiagnosis(uint8_t mask)
{
	uint8_t ret = readStatusReg(SYS_DIAG1, mask, 0);
	return ret^(TLE9563_STATUS_INV_MASK&mask);
}

uint8_t Tle9563::getHBOverCurrent(HalfBridge hb)
{
	return getHBOverCurrent(static_cast<uint8_t>(hb));
}

uint8_t Tle9563::getHBOverCurrent(uint8_t hb)
{
	uint8_t reg = mHalfBridges[hb].ocReg;
	uint8_t mask = mHalfBridges[hb].ocMask;
	uint8_t shift = mHalfBridges[hb].ocShift;
	return readStatusReg(reg, mask, shift);
}

uint8_t Tle9563::getHBOpenLoad(HalfBridge hb)
{
	return getHBOpenLoad(static_cast<uint8_t>(hb));
}

uint8_t Tle9563::getHBOpenLoad(uint8_t hb)
{
	uint8_t reg = mHalfBridges[hb].olReg;
	uint8_t mask = mHalfBridges[hb].olMask;
	uint8_t shift = mHalfBridges[hb].olShift;
	return readStatusReg(reg, mask, shift);
}

void Tle9563::clearErrors()
{
	clearStatusReg(SYS_DIAG1);
	clearStatusReg(OP_ERROR_1_STAT);
	clearStatusReg(OP_ERROR_2_STAT);
	clearStatusReg(OP_ERROR_3_STAT);
	clearStatusReg(OP_ERROR_4_STAT);
	clearStatusReg(OP_ERROR_5_STAT);
	clearStatusReg(OP_ERROR_6_STAT);
}
