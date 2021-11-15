/*!
 * \name        TLE9563_BLDCM_rise-and-fall-time-regulation.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example lets you adapt the rise and falltime on a TLE9563 BLDC shield running a brushlessmotor with hall sensor position feedback. Feedback mode can be changed to sensorless if needed.
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
  Serial.begin(115200);
  Serial.println(" Infineon TLE9563 Gate Driver Configuration tool");

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);         // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.setLED(0,0,200);                                              // Set onboard RGB-LED to blue, indicating development mode.

  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;            // Set feedback mode to hall sensor
  MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_PERCENTAGE;         // Set speed mode to Dutycycle
  MyMotor.MotorParam.MotorPolepairs = 4;                                // only mandatory, if BLDC_RPM was selected

  MyMotor.configBLDCshield(AGC_ACTIVE);
  MyMotor.setupRiseFallTimeRegulation(HALFBRIDGE);
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
      Serial.println("forward");}
    if(in == 'e'){
       direction = 1;
       Serial.println("backward");}
    if(in == 's'){
      weakening = 0;
      Serial.println("Field weakening disabled");}
    if(in == 'w'){
      weakening = 1;
      Serial.println("Field weakening enabled");}
    if(in == 'h'){
      MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);
      Serial.println("Motor stopped");}
    if(in == 'g'){
      MyMotor.startBLDCM();
      Serial.println("Motor started");}
    MyMotor.setBLDCspeed(speed, direction, weakening);

    //==================== Adaptive Gate Charge control ======================
    if(in == 'u')
    {
      riseFallTimeReg_enable = 1;
      Serial.println("Rise- Fall-time regulation enabled");
      Serial.println("iChg:\t tRise:\t iDchg:\t tFall:");
    }
    if(in == 'j')
    {
      riseFallTimeReg_enable = 0;
      Serial.println("Rise- Fall-time regulation disabled");
    }

    //==================== Adaptive Gate Pre-charge control ======================
    if(in == 'i')
    {
      turnOnOffDelayReg_enable = 1;
      MyMotor.configBLDCshield(AGC_ACTIVE);
      Serial.println("Turn-on / -off delay regulation enabled");
      //Serial.println("iPchg:\t iPDchg:\t tDon:\t tDoff:");
    }
    if(in == 'k')
    {
      turnOnOffDelayReg_enable = 0;
      MyMotor.configBLDCshield(AGC_INACTIVE1);
      Serial.println("Turn-on / -off delay regulation disabled");
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
        Serial.print("\t ");
        Serial.print(tRise);
        Serial.print("\t ");
        Serial.print(iDischarge);
        Serial.print("\t ");
        Serial.println(tFall);
        blinktimer = millis();
    }
}