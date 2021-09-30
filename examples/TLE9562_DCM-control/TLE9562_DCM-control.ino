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

// Create an instance of DCMcontrol called 'MyMotor'. 
DCMcontrolIno MyMotor = DCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(" Infineon TLE9562 DC motor shield Testsketch");

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.configDCshield();
  MyMotor.setLED(0,100);                                                 // Switch on LED 2
  
  Serial.println("Init ready");

  MyMotor.setDCspeed(speed, direction, 3);
  MyMotor.startDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();                 // Adapt the speed with keyboard input in the serial monitor
    if(in == 'i')                               // print TLE and MOSFET infos
    {
       MyMotor.print_TFALL_TRISE(0b1111);       // print t_Fall and t_Rise times of all halfbridges
    }
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
    if(in == 'h')
    {
      MyMotor.stopDCM(BRAKEMODE_PASSIVE);
      Serial.println("Motor stopped");
    }
    if(in == 'g')
    {
      MyMotor.startDCM();
      Serial.println("Motor started");
    }

    MyMotor.setDCspeed(speed, direction, 3);
  }

  if(MyMotor.checkBLDCshield() )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                  // Switch on LED 1
  }

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}