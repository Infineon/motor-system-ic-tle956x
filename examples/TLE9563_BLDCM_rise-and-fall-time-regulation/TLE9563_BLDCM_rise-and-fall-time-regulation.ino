/*!
 * \name        TLE9563_BLDCM_rise-and-fall-time-regulation.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example lets you configure the rise- and falltime of a TLE9563 in a brushless motor application. Therefore a closed-loop algorithm is implemented,
 * that prints the actual rise- /falltime and the gate charge- /dischargecurrent. Start the regulation by pressing 'u' in the serial monitor.
 * As soon as the values converge, stop the regulation by pressing 'j'. Note down the final charge- / dischargecurrent and edit the define-page 'TLE9xxx.hpp' in the library.
 * 
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include <BLDCM-control-ino.hpp>

#define SPEED_INCREASE_STEP     50          // [1;127] speed step increase/decrease when pressing a key
#define HALFBRIDGE              PHASE1      // [PHASE1;Phase3] Select the phase on which you want to regulate Rise/Fall time
#define RFTREG_DELAY            500         // milliseconds

uint16_t speed = 400;
uint8_t direction = 0;
uint8_t weakening = 0;

uint8_t tRise, tFall, iCharge, iDischarge = 0;
uint32_t blinktimer = millis();
bool ledstatus = 0;
bool riseFallTimeReg_enable = 0;
bool turnOnOffDelayReg_enable = 0;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(250000);
  Serial.println(F(" Infineon TLE9563 Gate Driver Configuration tool"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);         // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  /**
   * setLED(red, green, blue)
   * each value is 10 bit [0;1023]
   */
  MyMotor.setLED(500,80,450);                                         // Set onboard RGB-LED to pink

/**
 * .MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL       for motors with hallsensor
 * .MotorParam.feedbackmode = BLDCMcontrol::BLDC_BEMF       for motors without hall sensor
 * ------------------------------------------------------------------------------------------------------
 * .MotorParam.speedmode    = BLDCMcontrol::BLDC_PERCENTAGE to set dutycycle as speed between 0 and 1000
 * .MotorParam.speedmode    = BLDCMcontrol::BLDC_RPM        to set the speed in rounds per minute
 */
  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;            // Set feedback mode
  MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_PERCENTAGE;         // Set speed mode
  MyMotor.MotorParam.MotorPolepairs = 4;                                // only mandatory, if BLDC_RPM was selected

  MyMotor.configBLDCshield(AGC_ACTIVE);
  MyMotor.setupRiseFallTimeRegulation(HALFBRIDGE);

  /**
   * Depending on what you selected in MotorParam.speedmode, the speed has a different meaning:
   * - if(.MotorParam.speedmode == BLDCMcontrol::BLDC_PERCENTAGE): input range [0;1000]
   * - if(.MotorParam.speedmode == BLDCMcontrol::BLDC_RPM): input range [0;2E32]
   * - direction can be 0 or 1.
   */
  MyMotor.setBLDCspeed(speed, direction);
  MyMotor.startBLDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();     // Adapt the speed with keyboard input in the serial monitor
    if(in == '+'){
       speed += SPEED_INCREASE_STEP;
       Serial.println(speed);}
    if(in == '-'){
      speed -= SPEED_INCREASE_STEP;
      Serial.println(speed);}
    if(in == 'd'){
      direction = 0;
      Serial.println(F("forward"));}
    if(in == 'e'){
       direction = 1;
       Serial.println(F("backward"));}
    if(in == 's'){
      weakening = 0;
      Serial.println(F("Field weakening disabled"));}
    if(in == 'w'){
      weakening = 1;
      Serial.println(F("Field weakening enabled"));}
    if(in == 'h'){
      MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);
      Serial.println(F("Motor stopped"));}
    if(in == 'g'){
      MyMotor.startBLDCM();
      Serial.println(F("Motor started"));}
    MyMotor.setBLDCspeed(speed, direction, weakening);

    //==================== Adaptive Gate Charge control ======================
    if(in == 'u')
    {
      riseFallTimeReg_enable = 1;
      Serial.println(F("Rise- Fall-time regulation enabled"));
      Serial.println(F("iChg:\t tRise:\t iDchg:\t tFall:"));
    }
    if(in == 'j')
    {
      riseFallTimeReg_enable = 0;
      Serial.println(F("Rise- Fall-time regulation disabled"));
    }

    //==================== Adaptive Gate Pre-charge control ======================
    if(in == 'i')
    {
      turnOnOffDelayReg_enable = 1;
      MyMotor.configBLDCshield(AGC_ACTIVE);
      Serial.println(F("Turn-on / -off delay regulation enabled"));
    }
    if(in == 'k')
    {
      turnOnOffDelayReg_enable = 0;
      MyMotor.configBLDCshield(AGC_INACTIVE1);
      Serial.println(F("Turn-on / -off delay regulation disabled"));
    }
  }

  MyMotor.serveBLDCshield();                // MUST BE CALLED HERE. This function does the BLDC commutation.

  if(MyMotor.checkTLEshield() )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(50,0,0);                 // Set onboard RGB-LED to red.
  }

  if(riseFallTimeReg_enable)
  {
      riseFallTimeRegulation();
  }
}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}

void riseFallTimeRegulation()
{
    if((millis() - blinktimer) > RFTREG_DELAY)
    { 
        MyMotor.riseFallTimeRegulation(HALFBRIDGE, &iCharge, &iDischarge, &tRise, &tFall);
        Serial.print(iCharge);
        Serial.print(F("\t "));
        Serial.print(tRise);
        Serial.print(F("\t "));
        Serial.print(iDischarge);
        Serial.print(F("\t "));
        Serial.println(tFall);
        
        blinktimer = millis();
    }
}