/*!
 * @file        DCM-control.cpp
 * @name        DCM-control.cpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to control DC motors using an instance of TLE9562
 * @ref         tle9563corelib
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

uint8_t DCMcontrol::configDCshield(uint8_t agc )
{
  controller->config(agc);
  controller->configInterruptMask();
}

uint8_t DCMcontrol::checkTLEshield()
{
  uint8_t returnvalue = 0;
  if(interrupt_status_changed)
  {
      returnvalue = controller->checkStatusInformationField();
      interrupt_status_changed = 0;
  }
  return returnvalue;
}

void DCMcontrol::setDCspeed(uint16_t speed, bool direction, uint8_t motorNumber)
{
    //speed = (speed * 256)/1024;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
    speed = (speed>>2);
    if(speed > 255) _DutyCycle = 255;
    else _DutyCycle = speed;

    if(motorNumber == 0b01)         // motor A
    {
        pwmA->ADCWrite(_DutyCycle);
        if(direction)
        {
            _HBstatus[PHASE1] = controller->ActivePWM;
            _HBstatus[PHASE2] = controller->ActiveGround;
            _Direction[OUT_A] = PWM1_TO_HB1;
        }
        else
        {
            _HBstatus[PHASE2] = controller->ActivePWM;
            _HBstatus[PHASE1] = controller->ActiveGround;
            _Direction[OUT_A] = PWM1_TO_HB2;
        }
        
    }

    else if(motorNumber == 0b10)    // motor B
    {
        pwmB->ADCWrite(_DutyCycle);
        if(direction)
        {
            _HBstatus[PHASE3] = controller->ActivePWM;
            _HBstatus[PHASE4] = controller->ActiveGround;
            _Direction[OUT_B] = PWM3_TO_HB3;
        }
        else
        {
            _HBstatus[PHASE4] = controller->ActivePWM;
            _HBstatus[PHASE3] = controller->ActiveGround;
            _Direction[OUT_B] = PWM3_TO_HB4;
        }
    }

    else if(motorNumber == 0b11)    // both motors
    {
        pwmA->ADCWrite(_DutyCycle);
        pwmB->ADCWrite(_DutyCycle);
        if(direction)
        {
            _HBstatus[PHASE1] = controller->ActivePWM;
            _HBstatus[PHASE2] = controller->ActiveGround;
            _HBstatus[PHASE3] = controller->ActivePWM;
            _HBstatus[PHASE4] = controller->ActiveGround;
            _Direction[OUT_A] = PWM1_TO_HB1;
            _Direction[OUT_B] = PWM3_TO_HB3;
        }
        else
        {
            _HBstatus[PHASE2] = controller->ActivePWM;
            _HBstatus[PHASE1] = controller->ActiveGround;
            _HBstatus[PHASE4] = controller->ActivePWM;
            _HBstatus[PHASE3] = controller->ActiveGround;
            _Direction[OUT_A] = PWM1_TO_HB2;
            _Direction[OUT_B] = PWM3_TO_HB4;
        }
    }

    if(_MotorStartEnable)
    {
        controller->setHalfbridge(_HBstatus[PHASE1], _HBstatus[PHASE2], _HBstatus[PHASE3], _HBstatus[PHASE4]);
        controller->setGenControl(_Direction[OUT_A], _Direction[OUT_B]);
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

void DCMcontrol::setupRiseFallTimeRegulation(uint8_t hb)
{
    _MotorStartEnable = 0;
    controller->init_AGC_Algorithm(hb);
    
    switch(hb){
        case PHASE1:   
            controller->setGenControl(PWM1_TO_HB1, 0);
            controller->setHalfbridge(controller->ActivePWM, controller->ActiveGround, controller->ActiveGround, controller->ActiveGround);
            break;
        case PHASE2:  
            controller->setGenControl(PWM1_TO_HB2, 0);
            controller->setHalfbridge(controller->ActiveGround, controller->ActivePWM, controller->ActiveGround, controller->ActiveGround);
            break;
        case PHASE3:
            controller->setGenControl(0, PWM3_TO_HB3);
            controller->setHalfbridge(controller->ActiveGround, controller->ActiveGround, controller->ActivePWM, controller->ActiveGround);
            break;
        case PHASE4:
            controller->setGenControl(0, PWM3_TO_HB4);
            controller->setHalfbridge(controller->ActiveGround, controller->ActiveGround, controller->ActiveGround, controller->ActivePWM);
            break;
    }
}

void DCMcontrol::riseFallTimeRegulation(uint8_t hb, uint8_t * iCharge, uint8_t * iDischarge, uint8_t * risetime, uint8_t * falltime)
{
    controller->emaCalculation(hb, risetime, falltime);
    controller->adaptiveHysteresisDecisionTree (hb, iCharge, iDischarge);
}

void DCMcontrol::setTrisefallTarget(uint8_t trise_tg, uint8_t tfall_tg)
{
    if((trise_tg < 64) && (tfall_tg < 64))
    {
        controller->m_trise_tg = trise_tg;
        controller->m_tfall_tg = tfall_tg;
    }
}