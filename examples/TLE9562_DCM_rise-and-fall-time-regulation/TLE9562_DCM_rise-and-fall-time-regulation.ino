/*!
 * \name        TLE9562_DCM_rise-and-fall-time-regulation.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example lets you configure the Rise- and Fall-time of a TLE9562. Therefor a closed-loop Algorithm is implemented,
 * that prints the actual Rise- /Falltime and the gate charge- /dischargecurrent. Start the Regulation by pressing 'w' in the serial monitor.
 * As soon as the values converge, stop the regulation by pressing 's'. Note down the final charge- / dischargecurrent and edit the define-page in the library.
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
  Serial.println(" Infineon TLE956x Gate Driver Configuration tool");

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.configDCshield();
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
       Serial.print("Dutycycle: ");
       Serial.println(speed);
    }
    if(in == '-')
    {
      speed -= SPEED_INCREASE_STEP;
      Serial.print("Dutycycle: ");
      Serial.println(speed);
    }
    MyMotor.setDCspeed(speed, direction, 3);

    //==================== Adaptive Gate Charge control ======================
    if(in == 'w')
    {
      riseFallTimeReg_enable = 1;
      Serial.println("Rise- Fall-time regulation enabled");
      Serial.println("iChg:\t iDchg:\t tRise:\t tFall:");
    }
    if(in == 's')
    {
      riseFallTimeReg_enable = 0;
      Serial.println("Rise- Fall-time regulation disabled");
    }

    //==================== Adaptive Gate Pre-charge control ======================
    if(in == 'q')
    {
      turnOnOffDelayReg_enable = 1;
      MyMotor.configDCshield(turnOnOffDelayReg_enable);
      Serial.println("Turn-on / -off delay regulation enabled");
      Serial.println("iPchg:\t iPDchg:\t tDon:\t tDoff:");
    }
    if(in == 'a')
    {
      turnOnOffDelayReg_enable = 0;
      MyMotor.configDCshield(turnOnOffDelayReg_enable);
      Serial.println("Turn-on / -off delay regulation disabled");
    }
  }

  if(MyMotor.checkTLEshield() > 0 )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                     // Switch on LED 1
  }

  if(riseFallTimeReg_enable)
  {
    MyMotor.riseFallTimeRegulation(HALFBRIDGE, iCharge, iDischarge, tRise, tFall);
    Serial.print(iCharge);
    Serial.print("\t ");
    Serial.print(iDischarge);
    Serial.print("\t ");
    Serial.print(tRise);
    Serial.print("\t ");
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