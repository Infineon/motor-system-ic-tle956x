/*!
 * \name        TLE9563_BLDCM-control.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example runs a brushlessmotor with hall sensor position feedback using a TLE9563 BLDC control shield.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <BLDCM-control-ino.hpp>

#define SPEED_INCREASE_STEP     50          // [1;127] speed step increase/decrease when pressing a key

uint16_t speed = 400;
uint8_t direction = 0;
uint8_t weakening = 0;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(F(" Infineon TLE9563 BLDC shield Testsketch"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.setLED(0,20,0);                                                // Set onboard RGB-LED to low-bright green.

  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;             // Set feedback mode to hall sensor
  MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_PERCENTAGE;          // Set speed mode to Dutycycle
  MyMotor.MotorParam.MotorPolepairs = 4;                                 // only mandatory, if BLDC_RPM was selected

  MyMotor.configBLDCshield();

   /**
   * Depending on what you selected in MotorParam.speedmode, the speed has a different meaning:
   * if(MotorParam.speedmode == BLDCMcontrol::BLDC_PERCENTAGE): input range [0;1000]
   * if(MotorParam.speedmode == BLDCMcontrol::BLDC_RPM): input range [0;2E32]
   */
  MyMotor.setBLDCspeed(speed, direction);
  MyMotor.startBLDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();     // Adapt the speed with keyboard input in the serial monitor
    if(in == '+'){
       speed += SPEED_INCREASE_STEP;
       Serial.println(speed);}
    if(in == '-'){
      speed -= SPEED_INCREASE_STEP;
      Serial.println(speed);}
    if(in == 'd'){
      direction = 0;
      Serial.println(F("forward"));}
    if(in == 'e'){
       direction = 1;
       Serial.println(F("backward"));}
    if(in == 's'){
      weakening = 0;
      Serial.println(F("Field weakening disabled"));}
    if(in == 'w'){
      weakening = 1;
      Serial.println(F("Field weakening enabled"));}
    if(in == 'h'){
      MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);
      Serial.println(F("Motor stopped"));}
    if(in == 'g'){
      MyMotor.startBLDCM();
      Serial.println(F("Motor started"));}
    MyMotor.setBLDCspeed(speed, direction, weakening);
  }

  MyMotor.serveBLDCshield();                // MUST BE CALLED HERE. This function does the BLDC commutation.

  if(MyMotor.checkTLEshield() )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(50,0,0);                 // Set onboard RGB-LED to red.
  }

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}