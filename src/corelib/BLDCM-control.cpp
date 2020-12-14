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
  rpmtimer = NULL;
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
  rpmtimer = NULL;
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
  rpmtimer->init();
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
  rpmtimer->stop();
}

uint8_t BLDCMcontrol::serveBLDCshield(void)
{
  if(_MotorStartEnable == 1)
  {
    if(MotorParam.speedmode == TLE_RPM) PI_Regulator_DoWork();              // Set the Dutycycle within this function

    switch(MotorParam.feedbackmode)
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
        PrintErrorMessage(PARAMETER_MISSING); // Error: MotorMode not yet defined
        return 0;
    }
  }
}

uint8_t BLDCMcontrol::configBLDCshield()
{
  // TODO: Do the whole TLE setting here

  // Amount of steps for one full Revolution
  _NumberofSteps = (float) MotorParam.MotorPolepairs * 6.0;
}

void BLDCMcontrol::setLED(uint16_t red, uint16_t green, uint16_t blue)
{
  controller->setHSS(green, red, blue);
}

void BLDCMcontrol::setBLDCspeed(uint32_t speed, bool direction, uint8_t fieldweakening = 0)
{
  switch(MotorParam.speedmode)
  {
    case TLE_PERCENTAGE:
      speed = (speed * 255)/1000;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
      if(speed > 255) _DutyCycle = 255;
      else _DutyCycle = speed;

      break;
    case TLE_RPM:
      _RefRPM = speed;
      break;
    case TLE_POSITION:
      // something for FOC
      break;
    default:
      PrintErrorMessage(PARAMETER_MISSING);
      break;
  }
  _Direction = direction;
  _FieldWeakening = fieldweakening;
}

void BLDCMcontrol::StartBLDCM(void)
{
  if(MotorParam.feedbackmode == TLE_BEMF)
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

  else if(MotorParam.feedbackmode == TLE_HALL)
  {
    if(MotorParam.speedmode == TLE_RPM) _DutyCycle = 80; 
    _oldHall = ReadHallSensor();
    UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldHall] );
  }

  if(MotorParam.speedmode == TLE_RPM)
  {
    if(MotorParam.MotorPolepairs == 0) PrintErrorMessage(PARAMETER_MISSING);
    rpmtimer->start();
  }

  _LastBLDCspeed = 1;
  _MotorStartEnable = 1;            // Enable the use of serveBLDCshield()
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
  _LastBLDCspeed = 0;
  _MotorStartEnable = 0;        // Prohibit the use of serveBLDCshield()
}

uint8_t BLDCMcontrol::DoBEMFCommutation(void)
{
  if(_LastBLDCspeed == 0)         // Fallback, if StartBLDC() was not called in main user code.
  {
    StartBLDCM();
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
  if(_LastBLDCspeed == 0)         // Fallback, if StartBLDC() was not called in main user code.
  {
    StartBLDCM();
    return 1;
  }                           
                                //If motor is running, wait for the next flag from hall sensor
  if(WaitForCommutation() )
  {
    _oldHall = _latestHall;
    UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldHall] );
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
      return 0;
    }
  }
}

void BLDCMcontrol::PI_Regulator_DoWork()
{
  uint32_t Elapsed = 0;
  rpmtimer->elapsed(Elapsed);
  if (Elapsed > PI_UPDATE_INTERVAL)
  {
    float RPM = 0.0;
    float steps = (float) _StepCounter;
    // Formula for 100ms intervall: RPM = (Hallcounts / (6 * MotorPolepairs)) * 10 * 60
    
    //RPM = (steps/ _NumberofSteps) * 600.0;                 // Alternate function accurancy +-25 RPM
    RPM = (steps/ _NumberofSteps) * (60000.0 / Elapsed);     // Very precise but needs more calc power

    float Error = _RefRPM - RPM;
    if(_DutyCycle < 240) _PI_Integral = _PI_Integral + Error;
    float pwm = MotorParam.PI_Reg_P * Error + MotorParam.PI_Reg_I * _PI_Integral;
    //Limit PWM
    if (pwm > 240) pwm = 240;
    if (pwm < 30) pwm = 30;
    _DutyCycle = (uint8_t) pwm;    
    _StepCounter = 0;

    rpmtimer->start();
  }
   _StepCounter ++;
}

uint8_t BLDCMcontrol::ReadHallSensor(void)
{
   uint8_t hallpattern = 0;
   hallpattern = ( ((hallA->read())<<2) | ((hallB->read())<<1) | (hallC->read()) );
   _latestHall = hallpattern;
   return hallpattern;
}

uint8_t BLDCMcontrol::ReadBEMFSensor(void)
{
   uint8_t hallpattern = 0;
   hallpattern = ( ((bemfU->read())<<2) | ((bemfV->read())<<1) | (bemfW->read()) );
   _latestHall = hallpattern;
   return hallpattern;
}

uint8_t BLDCMcontrol::CommutateHallBLDC(uint8_t dutycycle, bool hallsensor)
{
  _DutyCycle = dutycycle;
   _Commutation++ ;
  if (_Commutation==6) _Commutation=0;
  UpdateHardware(_Commutation);
  timer->delayMilli(200);
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