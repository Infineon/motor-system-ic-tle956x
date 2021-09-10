/*!
 * \file        DCM-control.cpp
 * \name        DCM-control.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic common functions to control DC motors using an instance of TLE9562
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "DCM-control.hpp"

DCMcontrol::DCMcontrol(void)
{
    pwmA = NULL;
	pwmB = NULL;
    timer = NULL;

    
}

DCMcontrol::~DCMcontrol(void)
{
    pwmA = NULL;
	pwmB = NULL;
    timer = NULL;
}

void DCMcontrol::begin(void)
{
    controller->begin();
    controller->config();

	pwmA->init();
	pwmB->init();
	pwmA->setWriteFrequency(1);
	pwmB->setWriteFrequency(1);

	timer->init();
}

void DCMcontrol::end(void)
{
    pwmA->deinit();
	pwmB->deinit();

	timer->stop();
}

uint8_t DCMcontrol::configDCshield(void)
{
  // TODO: Do the whole TLE setting here
  controller->configInterruptMask();
}

void DCMcontrol::setDCspeed(uint32_t speed, bool direction, uint8_t motorNumber)
{
    if(motorNumber == 0b01)    // motor A
    {
        pwmA->ADCWrite(speed);
    }

    else if(motorNumber == 0b10)    // motor B
    {
        pwmB->ADCWrite(speed);
    }

    else if(motorNumber == 0b11)    // both motors
    {
        pwmA->ADCWrite(speed);
        pwmB->ADCWrite(speed);
    }
}