/*!
 * \file        TLE9563-platf-wiced.cpp
 * \name        TLE9563-platf-wiced.cpp - Cypress Hardware Abstraction Layer
 * \author      Infineon Technologies AG
 * \copyright   2019 Infineon Technologies AG
 * \version     2.0.0
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "TLE9563-platf-wiced.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

Tle9563Wiced::Tle9563Wiced(void):Tle9563()
{
	Tle9563::en = new GPIOWiced(TLE9563_PIN_EN, OUTPUT_PUSH_PULL, GPIOWiced::POSITIVE );
	Tle9563::cs = new GPIOWiced(TLE9563_PIN_CS1, OUTPUT_PUSH_PULL, GPIOWiced::POSITIVE );
	Tle9563::timer = new TimerWiced();
	Tle9563::sBus = new SPICWiced();
}

Tle9563Wiced::Tle9563Wiced(void* bus, uint8_t csPin):Tle9563()
{
}


void Tle9563Wiced::begin(void)
{
	begin(TLE9563_PIN_CS1);
}

void Tle9563Wiced::begin(uint8_t csPin)
{
	mEnabled = false;
	Tle9563::sBus->init();
	Tle9563::en->init();
	Tle9563::en->enable();
	Tle9563::cs->init();
	Tle9563::cs->enable();
	Tle9563::timer->init();
	mEnabled = true;
	init();
}

void Tle9563Wiced::end(void)
{
	mEnabled = false;
	Tle9563::en->disable();
	Tle9563::cs->disable();
	Tle9563::timer->stop();
	Tle9563::sBus->deinit();
}

#endif /** TLE9563_FRAMEWORK **/
