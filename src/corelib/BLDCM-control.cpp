/*!
 * \file        BLDCM-control.cpp
 * \name        BLDCM-control.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \version     1.0.0
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
    if(MotorParam.speedmode == BLDC_RPM) PI_Regulator_DoWork();              // Set the Dutycycle within this function

    switch(MotorParam.feedbackmode)
    {
      case BLDC_BEMF:
        DoBEMFCommutation();
        return 1;
      case BLDC_HALL:
        DoHALLCommutation();
        return 1;
      case BLDC_FOC:
        // Error: BLDC_FOC not yet implemented
        return 0;
      default:
        PrintErrorMessage(PARAMETER_MISSING); // Error: MotorMode not yet defined
        return 0;
    }
  }
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

uint8_t BLDCMcontrol::configBLDCshield()
{
  // TODO: Do the whole TLE setting here

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
    case BLDC_PERCENTAGE:
      speed = (speed * 255)/1000;          // TODO: 0.255 = (ReadAnalogWriteAccuracy() / 1000)
      if(speed > 255) _DutyCycle = 255;
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
  _DutyCycle = OPEN_LOOP_DUTYCYCLE;
  uint16_t i = 5000;
  uint8_t CommStartup = 0;
  while (i>1200)
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
    i=i-200;
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
    _DutyCycle = RPM_DUTYCYCLE_AT_START; 
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
    UpdateHardware( BEMFPattern[ 0 ][_Direction][_latestPattern] );   // No field weakening range possible in BEMF mode
    if(MotorParam.speedmode == BLDC_RPM) _StepCounter ++;
    timer->start();
    return 2;
  }
  else if( Elapsed > TIMEOUT)   // Does not work very well, as the BEMFPattern changes quickly when motor is blocked
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
    UpdateHardware( HallPattern[_FieldWeakening][_Direction][_oldPattern] );
    if(MotorParam.speedmode == BLDC_RPM) _StepCounter ++;
    timer->start();
    return 2;
  }
  else if( Elapsed > TIMEOUT)
  {
    stopBLDCM(BRAKEMODE_PASSIVE);
    return 0;
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
    if(_DutyCycle < 250) _PI_Integral = _PI_Integral + Error;
    float pwm = MotorParam.PI_Reg_P * Error + MotorParam.PI_Reg_I * _PI_Integral;
    //Limit PWM
    if (pwm > DUTYCYCLE_TOP_LIMIT) pwm = DUTYCYCLE_TOP_LIMIT;
    if (pwm < DUTYCYCLE_BOTTOM_LIMIT) pwm = DUTYCYCLE_BOTTOM_LIMIT;
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

void BLDCMcontrol::FindPolepairs(uint16_t delay, bool hallsensor)
{
	uint8_t Hallpattern = 0;
	uint8_t Counter = 0;
	uint8_t Magnetpoles = 0;
	uint8_t Magnetpolepairs = 0;
	uint8_t in;

	Serial.println("Mark a point at the rotation axis of your motor in order to determine its position.");
    timer->delayMilli(1000);

    Serial.println("Press enter to bring motor in start position");
    while(Serial.available() == 0);
    in = Serial.read();
    Hallpattern = CommutateHallBLDC(DUTYCYCLE_SINGLE_STEP, hallsensor);     //go in initial position
    timer->delayMilli(800);
    stopBLDCM(BRAKEMODE_PASSIVE);

    Serial.println(" ");
    Serial.println("Press enter to start the measurement.");
    Serial.println("Press enter again to stop the measurement, when the motor did one full revolution");
    Serial.println(" ");
    while(Serial.available() == 0);
    in = Serial.read();

	Serial.println("Step | Commutation | HallpatternDEC | HallpatternBIN");
    while(Serial.available() == 0)
    {
        Counter ++;
        Hallpattern = CommutateHallBLDC(DUTYCYCLE_SINGLE_STEP, hallsensor);
        Serial.print(Counter);						// Print Step
        if(Counter < 10) Serial.print(" ");         //Align values
        Serial.print("          ");
        Serial.print(_Commutation);					// Print Commutation
        Serial.print("          ");	
        if(hallsensor == 1)
        {
            Serial.print(Hallpattern);				// Print hall pattern decimal
            Serial.print("          ");
			PrintBinary(3, Hallpattern);			// Print hall pattern binary
			Serial.println("");
        }
        else Serial.println(" / ");
        timer->delayMilli(delay);
    }
    in = Serial.read();			// empty serial buffer

	stopBLDCM(BRAKEMODE_PASSIVE);

	// Evaluation
    if((Counter % 2) == 1)
    {
        Serial.println("Please try again, it must be a even number, when you stop the motor");
        // BLDCM_APP_LOG("Please try again, it must be a even number, when you stop the motor\n");
        // tle9563_log.print("asdfasdf %u", value );
    }
    else if((Counter % 6) > 0)
    {
        Serial.println("Please try again, it must be a multiple of 6");
    }
    else
    {
        Magnetpolepairs = Counter/6;
        Magnetpoles = Magnetpolepairs * 2;
        Serial.print("Your motor has ");
        // TLE9563_LOG_MSG_VAL("Your motor has  %u", Magnetpolepairs);
        Serial.print(Magnetpolepairs);
        Serial.print(" polepairs (equal to ");
        Serial.print(Magnetpoles);
        Serial.println(" poles)");
    }
    Serial.println("======================================================================");
    Serial.println("");

	timer->delayMilli(3000);
    Counter = 0;
}

void BLDCMcontrol::PrintErrorMessage(_ErrorMessages msg)
{
    switch(msg)
    {
        case PARAMETER_MISSING:
            Serial.println("=> Error: For this operation mode one or more motor parameter(s) are missing! <=");
            break;
        case PARAMETER_OUT_OF_RANGE:
            Serial.println("=> Warning: A parameter is out of range! <=");
            break;
        default:
            break;
    }
    setLED(50,0,0);      // Set onboard RGB-LED to red
}