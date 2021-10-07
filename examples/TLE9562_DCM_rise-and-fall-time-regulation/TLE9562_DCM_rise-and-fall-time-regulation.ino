/*!
 * \name        TLE9562_DCM-control.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example runs up to two DC motors using the TLE9562 DC motor shield.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <DCM-control-ino.hpp>

uint16_t speed = 100;
uint8_t direction = 0;
uint8_t tRise, tFall = 0;
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
  
  Serial.println("Init ready");

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
      Serial.println("tRise: \t tFall:");
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
    MyMotor.setLED(100,0);                 // Switch on LED 1
  }

  if(riseFallTimeReg_enable)
  {
    MyMotor.riseFallTimeRegulation(HB1, tRise, tFall);
    Serial.print(tRise);
    Serial.print(" \t ");
    Serial.println(tFall);
  }

  blinkLED();
  delay(10);

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}

void blinkLED()
{
    if((millis() - blinktimer) > 1000)
    {
        ledstatus != ledstatus;
        MyMotor.setLED(0,ledstatus * 1000);
        blinktimer = millis();
    }
}