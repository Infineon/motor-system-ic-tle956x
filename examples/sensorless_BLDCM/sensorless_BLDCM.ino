/*!
 * \name        sensorless_BLDCM.ino
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This example runs a senorless brushlessmotor in BEMF mode using a TLE9563 BLDC control shield.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <BLDCM-control-ino.hpp>
#include <SPI.h>

#define DIRECTION 0
uint8_t DutyCycle = 80;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(" Infineon TLE9563 BLDC shield Testsketch");
  Serial.print(" Mode: ");
  Serial.println("sensorless BEMF");

  MyMotor.begin();
  MyMotor.setLED(0,20,0);      // Set onboard RGB-LED to low-bright green.
  MyMotor.setBLDCspeed(DutyCycle, DIRECTION);
  MyMotor.MotorParam.mode = BLDCMcontrolIno::TLE_BEMF;
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+') DutyCycle += 2;          // Adapt the speed with keyboard input in the serial monitor
    if(in == '-') DutyCycle -= 2;
    Serial.println(DutyCycle);
    MyMotor.setBLDCspeed(DutyCycle, DIRECTION);
  }

  MyMotor.serveBLDCshield();

}