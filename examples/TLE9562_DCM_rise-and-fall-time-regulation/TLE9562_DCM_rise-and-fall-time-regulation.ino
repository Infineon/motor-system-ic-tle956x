/*!
 * \name        TLE9562_DCM_rise-and-fall-time-regulation.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example lets you configure the rise- and falltime of a TLE9562. Therefore a closed-loop algorithm is implemented,
 * that prints the actual rise- /falltime and the gate charge- /dischargecurrent. Start the regulation by pressing 'u' in the serial monitor.
 * As soon as the values converge, stop the regulation by pressing 'j'. Note down the final charge- / dischargecurrent and edit the define-page in the library.
 * See the Github wiki for more detailed information.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <DCM-control-ino.hpp>

#define HALFBRIDGE              PHASE1      // [PHASE1;Phase4] Select the phase on which you want to regulate Rise/Fall time
#define SPEED_INCREASE_STEP     50          // [1;127] speed step increase/decrease when pressing a key
#define CONTROL_LOOP_DELAY      100         // [ms] time between regulation executions

uint16_t speed = 100;                       // start speed
uint8_t direction = 0;                      // direction can not be changed in this application, the value has no effect
uint8_t tRise, tFall, iCharge, iDischarge = 0;
uint32_t blinktimer = millis();
bool ledstatus = 0;
bool riseFallTimeReg_enable = 0;
bool turnOnOffDelayReg_enable = 0;

// Create an instance of DCMcontrol called 'MyMotor'. 
DCMcontrolIno MyMotor = DCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(F(" Infineon TLE9562 Gate Driver Configuration tool"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.configDCshield(AGC_ACTIVE);
  MyMotor.setLED(0,100);                                                 // Switch on LED 2
  MyMotor.setupRiseFallTimeRegulation(HALFBRIDGE);
  MyMotor.setDCspeed(speed, direction, 3);
}

void loop()
{
    if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();                 // Adapt the speed with keyboard input in the serial monitor
    if(in == '+')
    {
       speed += SPEED_INCREASE_STEP;
       Serial.print(F("Dutycycle: "));
       Serial.println(speed);
    }
    if(in == '-')
    {
      speed -= SPEED_INCREASE_STEP;
      Serial.print(F("Dutycycle: "));
      Serial.println(speed);
    }
    MyMotor.setDCspeed(speed, direction, 3);

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
      MyMotor.configDCshield(AGC_ACTIVE);
      Serial.println(F("Turn-on / -off delay regulation enabled"));
    }
    if(in == 'k')
    {
      turnOnOffDelayReg_enable = 0;
      MyMotor.configDCshield(AGC_INACTIVE1);
      Serial.println(F("Turn-on / -off delay regulation disabled"));
    }
  }

  if(MyMotor.checkTLEshield() > 0 )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                     // Switch on LED 1
  }

  if(riseFallTimeReg_enable)
  {
    MyMotor.riseFallTimeRegulation(HALFBRIDGE, &iCharge, &iDischarge, &tRise, &tFall);
    Serial.print(iCharge);
    Serial.print(F("\t "));
    Serial.print(tRise);
    Serial.print(F("\t "));
    Serial.print(iDischarge);
    Serial.print(F("\t "));
    Serial.println(tFall);
  }

  blinkLED();
  delay(CONTROL_LOOP_DELAY);

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}

void blinkLED()
{
    if((millis() - blinktimer) > 500)
    {
        ledstatus = !ledstatus;
        MyMotor.setLED(0,ledstatus * 1000);
        blinktimer = millis();
    }
}