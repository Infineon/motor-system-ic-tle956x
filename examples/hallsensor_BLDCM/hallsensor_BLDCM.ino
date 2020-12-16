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

uint16_t speed = 400;
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

  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::TLE_HALL;             // Set feedback mode to hall sensor
  MyMotor.MotorParam.speedmode = BLDCMcontrol::TLE_PERCENTAGE;          // Set speed mode to Dutycycle
  MyMotor.MotorParam.MotorPolepairs = 4;

  MyMotor.configBLDCshield();
  
  Serial.println("Init ready");

  MyMotor.setBLDCspeed(speed, direction);
  MyMotor.StartBLDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+') speed += 100;          // Adapt the speed with keyboard input in the serial monitor
    if(in == '-') speed -= 100;
    if(in == 'd') direction = 0;
    if(in == 'e') direction = 1;
    if(in == 's') weakening = 0;
    if(in == 'w') weakening = 1;
    Serial.println(speed);
    MyMotor.setBLDCspeed(speed, direction, weakening);
  }

  MyMotor.serveBLDCshield();

}