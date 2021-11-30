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

/**
*    1 (= 0b01): motor connected to PHASE1 and PHASE2
*    2 (= 0b10): motor connected to PHASE3 and PHASE4
*    3 (= 0b11): both motors
*/
#define MOTOR_OUTPUT            3            // [1;3]
#define SPEED_INCREASE_STEP     100          // [1;511] speed step increase/decrease when pressing a key

uint16_t speed = 400;
uint8_t direction = 0;

// Create an instance of DCMcontrol called 'MyMotor'. 
DCMcontrolIno MyMotor = DCMcontrolIno();

void setup()
{
  Serial.begin(250000);
  Serial.println(F(" Infineon TLE9562 DC motor control"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.configDCshield();

  /**
   * setLED(led1, led2)
   * both values are 10-bit [0;1023]
   */
  MyMotor.setLED(0,100);                                                 // Switch on LED 2

  /**
   * setDCspeed(speed, direction, motoroutput)
   * - speed is the dutycycle speed in 10-bit, values between [0;1023]
   * - direction can be [0;1]
   * - motoroutput is the motor you want to control [1;3]
   *    1 (= 0b01): motor connected to PHASE1 and PHASE2
   *    2 (= 0b10): motor connected to PHASE3 and PHASE4
   *    3 (= 0b11): both motors
   */
  MyMotor.setDCspeed(speed, direction, MOTOR_OUTPUT);
  MyMotor.startDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+')
    {
       speed += SPEED_INCREASE_STEP;
       Serial.println(speed);
    }
    if(in == '-')
    {
      speed -= SPEED_INCREASE_STEP;
      Serial.println(speed);
    }
    if(in == 'd')
    {
      direction = 0;
      Serial.println(F("forward"));
    }
    if(in == 'e')
    {
       direction = 1;
       Serial.println(F("backward"));
    }
    if(in == 'h')
    {
      MyMotor.stopDCM(BRAKEMODE_PASSIVE);
      Serial.println(F("Motor stopped"));
    }
    if(in == 'g')
    {
      MyMotor.startDCM();
      Serial.println(F("Motor started"));
    }

    MyMotor.setDCspeed(speed, direction, MOTOR_OUTPUT);
  }

  if(MyMotor.checkTLEshield() )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                  // Switch on LED 1
  }

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}