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

#define ADAPTIVE_GATE_CONTROL_PRECHARGE			0		// 0 = INACTIVE1; 1 = INACTIVE2; 2 = ACTIVE | Built in AGC

uint16_t speed = 100;
uint8_t direction = 0;
uint8_t tRise, tFall, iCharge, iDischarge = 0;
uint32_t blinktimer = millis();
bool ledstatus = 0;
bool riseFallTimeReg_enable = 0;

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
  MyMotor.setupRiseFallTimeRegulation(HB1);
  MyMotor.setDCspeed(speed, direction, 3);
}

void loop()
{
    if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();                 // Adapt the speed with keyboard input in the serial monitor
    if(in == '+')
    {
       speed += 50;
       Serial.println(speed);
    }
    if(in == '-')
    {
      speed -= 50;
      Serial.println(speed);
    }
    if(in == 'd')
    {
      direction = 0;
      Serial.println("forward");
    }
    if(in == 'e')
    {
       direction = 1;
       Serial.println("backward");
    }
    if(in == 'w')
    {
      riseFallTimeReg_enable = 1;
      Serial.println("Rise- Fall-time Regulation enabled");
      Serial.println("iChg:\t iDchg:\t tRise:\t tFall:");
    }
    if(in == 's')
    {
      riseFallTimeReg_enable = 0;
      Serial.println("Rise- Fall-time Regulation disabled");
    }

    MyMotor.setDCspeed(speed, direction, 3);
  }

  if(MyMotor.checkTLEshield() > 0 )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                     // Switch on LED 1
  }

  if(riseFallTimeReg_enable)
  {
    MyMotor.riseFallTimeRegulation(HB1, iCharge, iDischarge, tRise, tFall);
    Serial.print(iCharge);
    Serial.print("\t ");
    Serial.print(iDischarge);
    Serial.print("\t ");
    Serial.print(tRise);
    Serial.print("\t ");
    Serial.println(tFall);
  }

  blinkLED();
  delay(50);

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