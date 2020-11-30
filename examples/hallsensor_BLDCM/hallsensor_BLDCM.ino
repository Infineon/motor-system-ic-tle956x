/*!
 * \name        hallsensor_BLDCM.ino
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This example runs a brushlessmotor with hall sensor position feedback using a TLE9563 BLDC control shield.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <BLDCM-control-ino.hpp>
#include <SPI.h>

uint8_t DutyCycle = 80;
uint8_t direction = 0;
uint8_t weakening = 0;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(" Infineon TLE9563 BLDC shield Testsketch");
  Serial.print(" Mode: ");
  Serial.println("hall sensor");

  MyMotor.begin();
  MyMotor.setLED(0,20,0);      // Set onboard RGB-LED to low-bright green.
  MyMotor.setBLDCspeed(DutyCycle, direction);

  // Set feedback mode to hall sensor
  MyMotor.MotorParam.mode = BLDCMcontrolIno::TLE_HALL;
  
  Serial.println("Init ready");
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+') DutyCycle += 5;          // Adapt the speed with keyboard input in the serial monitor
    if(in == '-') DutyCycle -= 5;
    if(in == 'd') direction = 0;          // Adapt the speed with keyboard input in the serial monitor
    if(in == 'e') direction = 1;
    if(in == 's') weakening = 0;          // Adapt the speed with keyboard input in the serial monitor
    if(in == 'w') weakening = 1;
    Serial.println(DutyCycle);
    MyMotor.setBLDCspeed(DutyCycle, direction, weakening);
  }

  MyMotor.serveBLDCshield();

}