/*!
 * @file        BLDCM-control.cpp
 * @name        BLDCM-control.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * @ref         tle9563corelib
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
    if(MotorParam.speedmode == BLDC_RPM) PI_Regulator_DoWork();              // Set the Dutycycle within this function

    switch(MotorParam.feedbackmode)
    {
      case BLDC_BEMF:
        DoBEMFCommutation();
        break;
      case BLDC_HALL:
        DoHALLCommutation();
        break;
      case BLDC_FOC:
        // Error: BLDC_FOC not yet implemented
        return 0;
      default:
        PrintErrorMessage(PARAMETER_MISSING); // Error: MotorMode not yet defined
        return 0;
    }

    if(_RFTReg_enable)            // Do the Rise-/Falltime regulation if enabled
    {
      switch(_RFTReg_phase)       // Make sure the Rise/Falltime regulation is only applied , when the correct Phase is in active PWM mode
      {
        case PHASE1:
          if((_commutationStep == 0) || (_commutationStep == 1)) _RFTReg_enable = 0;
          break;
        case PHASE2:
          if((_commutationStep == 2) || (_commutationStep == 3)) _RFTReg_enable = 0;
          break;
        case PHASE3:
          if((_commutationStep == 4) || (_commutationStep == 5)) _RFTReg_enable = 0;
          break;
      }
      if(_RFTReg_enable == 0)
      {
        controller->emaCalculation(_RFTReg_phase, _RFT_risetime, _RFT_falltime);
        controller->adaptiveHysteresisDecisionTree (_RFTReg_phase, _RFT_iCharge, _RFT_iDischarge);
      }
    }
  }
  return 1;
}

uint8_t BLDCMcontrol::checkTLEshield()
{
  uint8_t returnvalue = 0;
  if(interrupt_status_changed)
  {
      returnvalue = controller->checkStatusInformationField();
      interrupt_status_changed = 0;
  }
  return returnvalue;
}

uint8_t BLDCMcontrol::configBLDCshield(uint8_t agc = AGC_ACTIVE)
{
  controller->config(agc);

  // Amount of steps for one full Revolution
  _NumberofSteps = (float) MotorParam.MotorPolepairs * 6.0;
  controller->configInterruptMask();
}

void BLDCMcontrol::setLED(uint16_t red, uint16_t green, uint16_t blue)
{
  controller->setHSS(green, red, blue);
}

void BLDCMcontrol::setBLDCspeed(uint32_t speed, bool direction, uint8_t fieldweakening = 0)
{
  switch(MotorParam.speedmode)
  {
    case BLDC_DUTYCYCLE:
      //speed = (speed * CONF_DUTYCYCLE_TOP_LIMIT) / 1000;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
      speed = (speed>>2);
      if(speed > CONF_DUTYCYCLE_TOP_LIMIT) _DutyCycle = CONF_DUTYCYCLE_TOP_LIMIT;
      else _DutyCycle = speed;

      break;
    case BLDC_RPM:
      _RefRPM = speed;
      break;
    case BLDC_POSITION:
      // something for FOC
      break;
    default:
      PrintErrorMessage(PARAMETER_MISSING);
      break;
  }
  _Direction = direction;
  _FieldWeakening = fieldweakening;
}

void BLDCMcontrol::startBLDCM(void)
{
  //if(MotorParam.feedbackmode == BLDC_BEMF)
  uint8_t dt_prev = _DutyCycle;
  _DutyCycle = CONF_OPEN_LOOP_DUTYCYCLE;
  uint16_t i = CONF_OPEN_LOOP_DELAY_START;
  uint8_t CommStartup = 0;
  while (i>CONF_OPEN_LOOP_DELAY_LIMIT)
  {
    timer->delayMicro(i);
    _Commutation = CommStartup;
    UpdateHardware(CommStartup);
    if(_Direction == 0)
    {
      CommStartup++;
      if (CommStartup==6) CommStartup=0;
    }
    if(_Direction == 1)
    {
      if (CommStartup==0) CommStartup=6;
      CommStartup--;
    }
    i=i-CONF_OPEN_LOOP_DELAY_SLOPE;
  }
  _DutyCycle = dt_prev;
  timer->start();

  if(MotorParam.feedbackmode == BLDC_HALL)
  {
    _oldPattern = ReadHallSensor();
    UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldPattern] );
    timer->start();
  }

  if(MotorParam.speedmode == BLDC_RPM)
  {
    _DutyCycle = CONF_RPM_DUTYCYCLE_AT_START; 
    if(MotorParam.MotorPolepairs == 0) PrintErrorMessage(PARAMETER_MISSING);
    rpmtimer->start();
    //_DutyCycle = 100;
  }

  _LastBLDCspeed = 1;
  _MotorStartEnable = 1;            // Enable the use of serveBLDCshield()
}

uint8_t BLDCMcontrol::stopBLDCM(uint8_t brakemode)
{
  if(brakemode == BRAKEMODE_PASSIVE)
  {
    controller->setHalfbridge(controller->Floating, controller->Floating, controller->Floating);
  }
  else if(brakemode == BRAKEMODE_ACTIVE)
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
    startBLDCM();
    return 1;
  }
  uint32_t Elapsed = 0;
  timer->elapsed(Elapsed);
  _latestPattern = ReadBEMFSensor();

  if(_latestPattern != _oldPattern)
  {
    _oldPattern = _latestPattern;
    _commutationStep = BEMFPattern[ 0 ][_Direction][_latestPattern];  // No field weakening range possible in BEMF mode
    UpdateHardware( _commutationStep );
    if(MotorParam.speedmode == BLDC_RPM) _StepCounter ++;
    timer->start();
    return 2;
  }
  else if( Elapsed > CONF_TIMEOUT)   // Does not work very well, as the BEMFPattern changes quickly when motor is blocked
  {
    stopBLDCM(BRAKEMODE_PASSIVE);
    return 0;
  }

}

uint8_t BLDCMcontrol::DoHALLCommutation(void)
{
  if(_LastBLDCspeed == 0)         // Fallback, if StartBLDC() was not called in main user code.
  {
    startBLDCM();
    return 1;
  }                           
  uint32_t Elapsed = 0;
  timer->elapsed(Elapsed);
  _latestPattern = ReadHallSensor();

  if(_latestPattern != _oldPattern)
  {
    _oldPattern = _latestPattern;
    _commutationStep = HallPattern[_FieldWeakening][_Direction][_oldPattern];
    UpdateHardware( _commutationStep );
    if(MotorParam.speedmode == BLDC_RPM) _StepCounter ++;
    timer->start();
    return 2;
  }
  else if( Elapsed > CONF_TIMEOUT)
  {
    stopBLDCM(BRAKEMODE_PASSIVE);
    return 0;
  }
}

void BLDCMcontrol::PI_Regulator_DoWork()
{
  uint32_t Elapsed = 0;
  rpmtimer->elapsed(Elapsed);
  if (Elapsed > CONF_PI_UPDATE_INTERVAL)
  {
    float RPM = 0.0;
    float steps = (float) _StepCounter;
    // Formula for 100ms intervall: RPM = (Hallcounts / (6 * MotorPolepairs)) * 10 * 60
    
    //RPM = (steps/ _NumberofSteps) * 600.0;                 // Alternate function accurancy +-25 RPM
    RPM = (steps/ _NumberofSteps) * (60000.0 / Elapsed);     // Very precise but needs more calc power

    float Error = _RefRPM - RPM;
    if(_DutyCycle < CONF_DUTYCYCLE_TOP_LIMIT) _PI_Integral = _PI_Integral + Error;
    float pwm = MotorParam.PI_Reg_P * Error + MotorParam.PI_Reg_I * _PI_Integral;
    //Limit PWM
    if (pwm > CONF_DUTYCYCLE_TOP_LIMIT) pwm = CONF_DUTYCYCLE_TOP_LIMIT;
    if (pwm < CONF_DUTYCYCLE_BOTTOM_LIMIT) pwm = CONF_DUTYCYCLE_BOTTOM_LIMIT;
    _DutyCycle = (uint8_t) pwm;    
    _StepCounter = 0;

    rpmtimer->start();
  }
}

uint8_t BLDCMcontrol::ReadHallSensor(void)
{
   uint8_t hallpattern = 0;
   hallpattern = ( ((hallA->read())<<2) | ((hallB->read())<<1) | (hallC->read()) );
   _latestPattern = hallpattern;
   return hallpattern;
}

uint8_t BLDCMcontrol::ReadBEMFSensor(void)
{
   uint8_t hallpattern = 0;
   hallpattern = ( ((bemfU->read())<<2) | ((bemfV->read())<<1) | (bemfW->read()) );
   _latestPattern = hallpattern;
   return hallpattern;
}

uint8_t BLDCMcontrol::commutateHallBLDC(uint8_t dutycycle, uint8_t commutation_step, bool hallsensor)
{
  _DutyCycle = dutycycle;
  if ((commutation_step>= 0) && (commutation_step < 6)) UpdateHardware(commutation_step);
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

void BLDCMcontrol::PrintErrorMessage(_ErrorMessages msg)
{
    switch(msg)
    {
        case PARAMETER_MISSING:
            Serial.println(F("=> Error: For this operation mode one or more motor parameter(s) are missing! <="));
            break;
        case PARAMETER_OUT_OF_RANGE:
            Serial.println(F("=> Warning: A parameter is out of range! <="));
            break;
        default:
            break;
    }
    setLED(50,0,0);      // Set onboard RGB-LED to red
}

void BLDCMcontrol::setupRiseFallTimeRegulation(uint8_t hb)
{
    _MotorStartEnable = 0;
    controller->init_AGC_Algorithm(hb); 
}

void BLDCMcontrol::riseFallTimeRegulation(uint8_t hb, uint8_t * iCharge, uint8_t * iDischarge, uint8_t * risetime, uint8_t * falltime)
{
  _RFTReg_enable = 1;
  _RFTReg_phase = hb;
  _RFT_iCharge = iCharge;
  _RFT_iDischarge = iDischarge;
  _RFT_risetime = risetime;
  _RFT_falltime = falltime;
}

void BLDCMcontrol::setTrisefallTarget(uint8_t trise_tg, uint8_t tfall_tg)
{
    if((trise_tg < 64) && (tfall_tg < 64))
    {
        controller->m_trise_tg = trise_tg;
        controller->m_tfall_tg = tfall_tg;
    }
}

float BLDCMcontrol::getCurrent(void)
{
  float cso_voltage = controller->getCSOVoltage();
  float current = (cso_voltage * 1000) / SHUNT_RESISTOR_VALUE;
  return current;
}

