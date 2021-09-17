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

void DCMcontrol::setDCspeed(uint16_t speed, bool direction, uint8_t motorNumber)
{
    speed = (speed * 255)/1000;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
    if(speed > 255) _DutyCycle = 255;
    else _DutyCycle = speed;

    if(_MotorStartEnable)
    {
        if(motorNumber == 0b01)         // motor A
        {
            pwmA->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[0] = controller->ActivePWM;
                _HBstatus[1] = controller->ActiveGround;
            }
            else
            {
                _HBstatus[1] = controller->ActivePWM;
                _HBstatus[0] = controller->ActiveGround;
            }
            
        }

        else if(motorNumber == 0b10)    // motor B
        {
            pwmB->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[2] = controller->ActivePWM;
                _HBstatus[3] = controller->ActiveGround;
            }
            else
            {
                _HBstatus[3] = controller->ActivePWM;
                _HBstatus[2] = controller->ActiveGround;
            }
        }

        else if(motorNumber == 0b11)    // both motors
        {
            pwmA->ADCWrite(_DutyCycle);
            pwmB->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[0] = controller->ActivePWM;
                _HBstatus[1] = controller->ActiveGround;
                _HBstatus[2] = controller->ActivePWM;
                _HBstatus[3] = controller->ActiveGround;
            }
            else
            {
                _HBstatus[1] = controller->ActivePWM;
                _HBstatus[0] = controller->ActiveGround;
                _HBstatus[3] = controller->ActivePWM;
                _HBstatus[2] = controller->ActiveGround;
            }
        }
        controller->setHalfbridge(_HBstatus[0], _HBstatus[1], _HBstatus[2], _HBstatus[3]);
    }
}

void DCMcontrol::startDCM(void)
{
    _MotorStartEnable = 1;
    controller->setHalfbridge(_HBstatus[0], _HBstatus[1], _HBstatus[2], _HBstatus[3]);
}

uint8_t DCMcontrol::stopDCM(uint8_t brakemode)
{
    _MotorStartEnable = 0;
    _HBstatus[0] = controller->ActiveGround;
    _HBstatus[1] = controller->ActiveGround;
    _HBstatus[2] = controller->ActiveGround;
    _HBstatus[3] = controller->ActiveGround;

    controller->setHalfbridge(_HBstatus[0], _HBstatus[1], _HBstatus[2], _HBstatus[3]);
    return 1;
}