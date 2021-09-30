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

uint8_t DCMcontrol::checkBLDCshield()
{
  uint8_t returnvalue = 0;
  if(interrupt_status_changed)
  {
    uint8_t ErrorCode = 0;
    uint16_t RegAddress = 0;
    uint16_t RegContent = 0;
    ErrorCode = controller->checkStatSUP(RegAddress, RegContent);
    if(ErrorCode > 0)
    {
      returnvalue = 1;
      controller->PrintTLEErrorMessage(ErrorCode, RegAddress, RegContent);
    }
    ErrorCode = controller->checkStatTHERM(RegAddress, RegContent);
    if(ErrorCode > 0)
    {
      returnvalue = 1;
      controller->PrintTLEErrorMessage(ErrorCode, RegAddress, RegContent);
    }
    ErrorCode = controller->checkStatHSS(RegAddress, RegContent);
    if(ErrorCode > 0)
    {
      returnvalue = 1;
      controller->PrintTLEErrorMessage(ErrorCode, RegAddress, RegContent);
    }
    ErrorCode = controller->checkStatDEV(RegAddress, RegContent);
    if(ErrorCode > 0)
    {
      returnvalue = 1;
      controller->PrintTLEErrorMessage(ErrorCode, RegAddress, RegContent);
    }
    interrupt_status_changed = 0;
  }
  return returnvalue;
}

void DCMcontrol::setDCspeed(uint16_t speed, bool direction, uint8_t motorNumber)
{
    //speed = (speed * 255)/1000;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
    if(speed > 255) _DutyCycle = 255;
    else _DutyCycle = speed;

    if(_MotorStartEnable)
    {
        if(motorNumber == 0b01)         // motor A
        {
            pwmA->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[HB1] = controller->ActivePWM;
                _HBstatus[HB2] = controller->ActiveGround;
                _Direction[OUT_A] = PWM1_TO_HB1;
            }
            else
            {
                _HBstatus[HB2] = controller->ActivePWM;
                _HBstatus[HB1] = controller->ActiveGround;
                _Direction[OUT_A] = PWM1_TO_HB2;
            }
            
        }

        else if(motorNumber == 0b10)    // motor B
        {
            pwmB->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[HB3] = controller->ActivePWM;
                _HBstatus[HB4] = controller->ActiveGround;
                _Direction[OUT_B] = PWM3_TO_HB3;
            }
            else
            {
                _HBstatus[HB4] = controller->ActivePWM;
                _HBstatus[HB3] = controller->ActiveGround;
                _Direction[OUT_B] = PWM3_TO_HB4;
            }
        }

        else if(motorNumber == 0b11)    // both motors
        {
            pwmA->ADCWrite(_DutyCycle);
            pwmB->ADCWrite(_DutyCycle);
            if(direction)
            {
                _HBstatus[HB1] = controller->ActivePWM;
                _HBstatus[HB2] = controller->ActiveGround;
                _HBstatus[HB3] = controller->ActivePWM;
                _HBstatus[HB4] = controller->ActiveGround;
                _Direction[OUT_A] = PWM1_TO_HB1;
                _Direction[OUT_B] = PWM3_TO_HB3;
            }
            else
            {
                _HBstatus[HB2] = controller->ActivePWM;
                _HBstatus[HB1] = controller->ActiveGround;
                _HBstatus[HB4] = controller->ActivePWM;
                _HBstatus[HB3] = controller->ActiveGround;
                _Direction[OUT_A] = PWM1_TO_HB2;
                _Direction[OUT_B] = PWM3_TO_HB4;
            }
        }
        controller->setHalfbridge(_HBstatus[HB1], _HBstatus[HB2], _HBstatus[HB3], _HBstatus[HB4]);
        controller->setGenControl(_Direction[OUT_A], _Direction[OUT_B]);
        //controller->setHalfbridge(controller->ActiveGround, controller->ActivePWM, controller->ActiveGround, controller->ActiveGround);
    }
}

void DCMcontrol::startDCM(void)
{
    _MotorStartEnable = 1;
    controller->setGenControl(_Direction[OUT_A], _Direction[OUT_B]);
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

void DCMcontrol::setLED(uint16_t led1, uint16_t led2)
{
    controller->setHSS(0, 0, led1, led2);
}

void DCMcontrol::print_TFALL_TRISE(uint8_t hb)
{
    float trise = 0;
    float tfall = 0;
    if(hb & 0x1)
    {
        controller->checkStat_TRISE_FALL(1, trise, tfall);
        Serial.print("t_RISE1: ");
        Serial.print(trise);
        Serial.print("ns, \t t_FALL1: ");
        Serial.print(tfall);
        Serial.println("ns");
    }
    if(hb & 0x2)
    {
        controller->checkStat_TRISE_FALL(2, trise, tfall);
        Serial.print("t_RISE2: ");
        Serial.print(trise);
        Serial.print("ns, \t t_FALL2: ");
        Serial.print(tfall);
        Serial.println("ns");
    }
    if(hb & 0x4)
    {
        controller->checkStat_TRISE_FALL(3, trise, tfall);
        Serial.print("t_RISE3: ");
        Serial.print(trise);
        Serial.print("ns, \t t_FALL3: ");
        Serial.print(tfall);
        Serial.println("ns");
    }
    if(hb & 0x8)
    {
        controller->checkStat_TRISE_FALL(4, trise, tfall);
        Serial.print("t_RISE4: ");
        Serial.print(trise);
        Serial.print("ns, \t t_FALL4: ");
        Serial.print(tfall);
        Serial.println("ns");
    }
    
}