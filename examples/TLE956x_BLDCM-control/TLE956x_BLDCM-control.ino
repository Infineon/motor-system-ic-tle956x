/*!
 * \name        TLE956x_BLDCM-control.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * \version     1.0.0
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

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.setLED(0,20,0);                                                // Set onboard RGB-LED to low-bright green.

  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;             // Set feedback mode to hall sensor
  MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_PERCENTAGE;          // Set speed mode to Dutycycle
  MyMotor.MotorParam.MotorPolepairs = 4;                                 // only mandatory, if BLDC_RPM was selected

  MyMotor.configBLDCshield();
  
  Serial.println("Init ready");

  MyMotor.setBLDCspeed(speed, direction);
  MyMotor.StartBLDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();     // Adapt the speed with keyboard input in the serial monitor
    if(in == '+'){
       speed += 100;
       Serial.println(speed);}
    if(in == '-'){
      speed -= 100;
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
      MyMotor.StopBLDCM(BRAKEMODE_PASSIVE);
      Serial.println("Motor stopped");}
    if(in == 'g'){
      MyMotor.StartBLDCM();
      Serial.println("Motor started");}
    MyMotor.setBLDCspeed(speed, direction, weakening);
  }

  MyMotor.serveBLDCshield();
  MyMotor.checkBLDCshield();           // Check, if interrupt flag was set and read Status register of TLE

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}