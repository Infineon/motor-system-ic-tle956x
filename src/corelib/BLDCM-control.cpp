/*!
 * \file        BLDCM-control.cpp
 * \name        BLDCM-control.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */


#include "BLDCM-control.hpp"


BLDCMcontrol::BLDCMcontrol(void)
{
	pwmU = NULL;
	pwmV = NULL;
	pwmU = NULL;
	bemfU = NULL;
	bemfV = NULL;
	bemfW = NULL;
	hallA = NULL;
	hallB = NULL;
	hallC = NULL;
  timer = NULL;
}

BLDCMcontrol::~BLDCMcontrol(void)
{
  pwmU = NULL;
	pwmV = NULL;
	pwmU = NULL;
	bemfU = NULL;
	bemfV = NULL;
	bemfW = NULL;
	hallA = NULL;
	hallB = NULL;
	hallC = NULL;
  timer = NULL;
}

void BLDCMcontrol::begin(void)
{
  controller->begin();
  controller->config();

	pwmU->init();
	pwmV->init();
	pwmW->init();
	pwmU->setWriteFrequency(1);
	pwmV->setWriteFrequency(1);
	pwmW->setWriteFrequency(1);

	bemfU->init();
	bemfV->init();
	bemfW->init();

	hallA->init();
	hallB->init();
	hallC->init();

	timer->init();
}

void BLDCMcontrol::end(void)
{
	pwmU->deinit();
	pwmV->deinit();
	pwmW->deinit();

	bemfU->deinit();
	bemfV->deinit();
	bemfW->deinit();

	hallA->deinit();
	hallB->deinit();
	hallC->deinit();

	timer->stop();
}

uint8_t BLDCMcontrol::serveBLDCshield(void)
{
  switch(MotorParam.mode)
  {
    case TLE_BEMF:
      DoBEMFCommutation();
      return 1;
    case TLE_HALL:
      DoHALLCommutation();
      return 1;
    case TLE_FOC:
      // Error: TLE_FOC not yet implemented
      return 0;
    default:
      // Error: MotorMode not yet defined
      return 0;
  }
}

uint8_t BLDCMcontrol::configBLDCshield(BLDCParameter MyParameters)
{
  //BLDCMotorMode = MyParameters.mode;
}

void BLDCMcontrol::setLED(uint16_t red, uint16_t green, uint16_t blue)
{
  controller->setHSS(green, red, blue);
}

void BLDCMcontrol::setBLDCspeed(uint8_t dutycycle, bool direction, uint8_t fieldweakening = 0)
{
  _DutyCycle = dutycycle;
  _Direction = direction;
  _FieldWeakening = fieldweakening;
}

void BLDCMcontrol::StartBLDCM(void)
{
  uint8_t dt_prev = _DutyCycle;
  _DutyCycle = 80;
  uint16_t i = 5000;
  uint8_t CommStartup = 0;
  while (i>1200)
  {
    timer->delayMicro(i);
    _Commutation = CommStartup;
    UpdateHardware(CommStartup);
    CommStartup++;
    if (CommStartup==6) CommStartup=0;
    i=i-200;
  }
   _DutyCycle = dt_prev;
}

uint8_t BLDCMcontrol::StopBLDCM(uint8_t brakemode)
{
  if(brakemode == PASSIVE)
  {
    controller->setHalfbridge(controller->Floating, controller->Floating, controller->Floating);
  }
  else if(brakemode == ACTIVE)
  {
    controller->setHalfbridge(controller->ActiveGround, controller->ActiveGround, controller->ActiveGround);
  }
}

uint8_t BLDCMcontrol::DoBEMFCommutation(void)
{
  if(_LastBLDCspeed == 0)
  {
    StartBLDCM();
    _LastBLDCspeed = 1;
    return 1;
  }

  switch (_Commutation) {
    case 0:
      if (bemfW->read()==1) {
        _Commutation = 1;
        UpdateHardware(_Commutation);
      }
      break;
    case 1:
      if (bemfV->read()==0) { 
        _Commutation=2;
        UpdateHardware(_Commutation);
      }
      break;
    case 2:
      if (bemfU->read()==1) { 
        _Commutation=3;
        UpdateHardware(_Commutation);
      }
      break;
    case 3:
      if (bemfW->read()==0) { 
        _Commutation=4;
        UpdateHardware(_Commutation);
      }
      break;
    case 4:
      if (bemfV->read()==1) { 
        _Commutation=5;
        UpdateHardware(_Commutation);
      }
      break;
    case 5:
      if (bemfU->read()==0) { 
        _Commutation=0;
        UpdateHardware(_Commutation);
      }
      break;
    default:
    break;
  }

  return 2;
}

uint8_t BLDCMcontrol::DoHALLCommutation(void)
{
  if(_DutyCycle > 10)
    {
      if(_LastBLDCspeed == 0)   //If motor is stopped, start it
      {
        _oldHall = ReadHallSensor();
        UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldHall] );
        _LastBLDCspeed = 1;
      }

      else                      //If motor is running, wait for the next flag from hall sensor
      {
        if(WaitForCommutation())
        {
          _oldHall = _latestHall;
          UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldHall] );
        }
      }
    }
  else
  {
    StopBLDCM(PASSIVE);
    _LastBLDCspeed = 0;
  } 
}

bool BLDCMcontrol::WaitForCommutation(void)
{
  uint32_t Elapsed = 0;
  timer->start();
  while(1)
  {
    timer->elapsed(Elapsed);
    if(_oldHall != ReadHallSensor() ) return 1;
    else if( Elapsed > TIMEOUT)
    {
      StopBLDCM(PASSIVE);
      _LastBLDCspeed = 0;
      return 0;
    }
  }
}

uint8_t BLDCMcontrol::ReadHallSensor(void)
{
   uint8_t hallpattern = 0;
   hallpattern = ( ((hallA->read())<<2) | ((hallB->read())<<1) | (hallC->read()) );
   _latestHall = hallpattern;
   return hallpattern;
}

uint8_t BLDCMcontrol::CommutateHallBLDC(uint8_t dutycycle, bool hallsensor)
{
  _DutyCycle = dutycycle;
   _Commutation++ ;
  if (_Commutation==6) _Commutation=0;
  UpdateHardware(_Commutation);
  timer->delayMilli(100);
  if(hallsensor) return ReadHallSensor();
  else return 0;
}

void BLDCMcontrol::UpdateHardware(uint8_t CommutationStep)
{
	switch (CommutationStep) {
      case 0:
		    controller->setHalfbridge(controller->ActivePWM, controller->ActiveGround, controller->Floating);
        pwmU->ADCWrite(_DutyCycle);		  // PWM
        pwmV->ADCWrite(0);					    // GND
        pwmW->ADCWrite(0);					    // Floating
        break;

      case 1:
        controller->setHalfbridge(controller->ActivePWM, controller->Floating, controller->ActiveGround);
        pwmU->ADCWrite(_DutyCycle);		  // PWM
        pwmV->ADCWrite(0);					    // Floating
        pwmW->ADCWrite(0);					    // GND
        break;

      case 2:
        controller->setHalfbridge(controller->Floating, controller->ActivePWM, controller->ActiveGround);
        pwmU->ADCWrite(0);					    // Floating
        pwmV->ADCWrite(_DutyCycle);		  // PWM
        pwmW->ADCWrite(0);					    // GND
        break;

      case 3:
      	controller->setHalfbridge(controller->ActiveGround, controller->ActivePWM, controller->Floating);
        pwmU->ADCWrite(0);					    // GND
        pwmV->ADCWrite(_DutyCycle);	  	// PWM
        pwmW->ADCWrite(0);					    // Floating
        break;

      case 4:
        controller->setHalfbridge(controller->ActiveGround, controller->Floating, controller->ActivePWM);
        pwmU->ADCWrite(0);					    // GND
        pwmV->ADCWrite(0);					    // Floating
        pwmW->ADCWrite(_DutyCycle);	  	// PWM
        break;

      case 5:
       	controller->setHalfbridge(controller->Floating, controller->ActiveGround, controller->ActivePWM);
        pwmU->ADCWrite(0);					    // Floating
        pwmV->ADCWrite(0);					    // GND
        pwmW->ADCWrite(_DutyCycle);	  	// PWM
        break;

      default:
        break;
	}
}