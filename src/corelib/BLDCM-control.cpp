/*!
 * \file        BLDCM-control.cpp
 * \name        BLDCM-control.cpp - basic motor control functions
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to controll a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "BLDCM-control.hpp"

BLDCMcontrol::BLDCMcontrol(void)
{
	
}

BLDCMcontrol::~BLDCMcontrol(void)
{

}

void BLDCMcontrol::begin(void)
{
  controller.begin();
  controller.config();
}

void BLDCMcontrol::setLED(uint16_t red, uint16_t green, uint16_t blue)
{
  controller.setHSS(green, red, blue);
}

void BLDCMcontrol::setBLDCspeed(uint8_t dutycycle)
{
  if(_LastBLDCspeed == 0)
  {
    _DutyCycle = 80;
    uint16_t i = 5000;
    uint8_t CommStartup = 0;
    while (i>1200)
    {
      controller.timer->delayMicro(i);
      _Commutation = CommStartup;
      UpdateHardware(CommStartup);
      CommStartup++;
      if (CommStartup==6) CommStartup=0;
      i=i-200;
    }
    _LastBLDCspeed = 1;
  }

  _DutyCycle = dutycycle;
  DoBEMFCommutation();
}

void BLDCMcontrol::DoBEMFCommutation(void)
{

  switch (_Commutation) {
    case 0:
      if (controller.bemfW->read()==1) {
        _Commutation = 1;
        UpdateHardware(_Commutation);
      }
      break;
    case 1:
      if (controller.bemfV->read()==0) { 
        _Commutation=2;
        UpdateHardware(_Commutation);
      }
      break;
    case 2:
      if (controller.bemfU->read()==1) { 
        _Commutation=3;
        UpdateHardware(_Commutation);
      }
      break;
    case 3:
      if (controller.bemfW->read()==0) { 
        _Commutation=4;
        UpdateHardware(_Commutation);
      }
      break;
    case 4:
      if (controller.bemfV->read()==1) { 
        _Commutation=5;
        UpdateHardware(_Commutation);
      }
      break;
    case 5:
      if (controller.bemfU->read()==0) { 
        _Commutation=0;
        UpdateHardware(_Commutation);
      }
      break;
    default:
    break;
    }

/*
   switch (_Commutation) {
      case 0:
        if (digitalRead(7)==1) {
          _Commutation = 1;
          UpdateHardware(_Commutation);
        }
        break;
      case 1:
        if (digitalRead(6)==0) { 
          _Commutation=2;
          UpdateHardware(_Commutation);
        }
        break;
      case 2:
        if (digitalRead(5)==1) { 
          _Commutation=3;
          UpdateHardware(_Commutation);
        }
        break;
      case 3:
        if (digitalRead(7)==0) { 
          _Commutation=4;
          UpdateHardware(_Commutation);
        }
        break;
      case 4:
        if (digitalRead(6)==1) { 
          _Commutation=5;
          UpdateHardware(_Commutation);
        }
        break;
      case 5:
        if (digitalRead(5)==0) { 
          _Commutation=0;
          UpdateHardware(_Commutation);
        }
        break;
      default:
      break;
      
    }
    */
}
void BLDCMcontrol::UpdateHardware(uint8_t CommutationStep)
{
	switch (CommutationStep) {
      case 0:
		    controller.setHalfbridge(controller.ActivePWM, controller.ActiveGround, controller.Floating);
        controller.pwmU->ADCWrite(_DutyCycle);		// PWM
        controller.pwmV->ADCWrite(0);					    // GND
        controller.pwmW->ADCWrite(0);					    // Floating
        break;

      case 1:
        controller.setHalfbridge(controller.ActivePWM, controller.Floating, controller.ActiveGround);
        controller.pwmU->ADCWrite(_DutyCycle);		// PWM
        controller.pwmV->ADCWrite(0);					    // Floating
        controller.pwmW->ADCWrite(0);					    // GND
        break;

      case 2:
        controller.setHalfbridge(controller.Floating, controller.ActivePWM, controller.ActiveGround);
        controller.pwmU->ADCWrite(0);					    // Floating
        controller.pwmV->ADCWrite(_DutyCycle);		// PWM
        controller.pwmW->ADCWrite(0);					    // GND
        break;

      case 3:
      	controller.setHalfbridge(controller.ActiveGround, controller.ActivePWM, controller.Floating);
        controller.pwmU->ADCWrite(0);					    // GND
        controller.pwmV->ADCWrite(_DutyCycle);		// PWM
        controller.pwmW->ADCWrite(0);					    // Floating
        break;

      case 4:
        controller.setHalfbridge(controller.ActiveGround, controller.Floating, controller.ActivePWM);
        controller.pwmU->ADCWrite(0);					    // GND
        controller.pwmV->ADCWrite(0);					    // Floating
        controller.pwmW->ADCWrite(_DutyCycle);		// PWM
        break;

      case 5:
       	controller.setHalfbridge(controller.Floating, controller.ActiveGround, controller.ActivePWM);
        controller.pwmU->ADCWrite(0);					    // Floating
        controller.pwmV->ADCWrite(0);					    // GND
        controller.pwmW->ADCWrite(_DutyCycle);		// PWM
        break;

      default:
        break;
	}
}