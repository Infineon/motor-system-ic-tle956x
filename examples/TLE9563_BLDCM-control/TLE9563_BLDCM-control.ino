/*!
 * @name        TLE9563_BLDCM-control.ino
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This example runs a brushlessmotor with hall sensor position feedback using a TLE9563 BLDC control shield.
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
  Serial.begin(250000);
  delay(100);
  Serial.println(F(" Infineon TLE9563 BLDC motor control"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  /**
   * setLED(red, green, blue)
   * each value is 10 bit [0;1023]
   */
  MyMotor.setLED(0,20,0);                                                // Set onboard RGB-LED to low-bright green.

/**
 * .MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL       for motors with hallsensor
 * .MotorParam.feedbackmode = BLDCMcontrol::BLDC_BEMF       for motors without hall sensor
 * ------------------------------------------------------------------------------------------------------
 * .MotorParam.speedmode    = BLDCMcontrol::BLDC_DUTYCYCLE to set dutycycle as speed between 0 and 1000
 * .MotorParam.speedmode    = BLDCMcontrol::BLDC_RPM        to set the speed in rounds per minute
 */
  MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;             // Set feedback mode
  MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_DUTYCYCLE;          // Set speed mode
  MyMotor.MotorParam.MotorPolepairs = 4;                                 // only mandatory, if BLDC_RPM was selected

  MyMotor.configBLDCshield();

   /**
   * Depending on what you selected in MotorParam.speedmode, the speed has a different meaning:
   * - if(.MotorParam.speedmode == BLDCMcontrol::BLDC_DUTYCYCLE): input range [0;1000]
   * - if(.MotorParam.speedmode == BLDCMcontrol::BLDC_RPM): input range [0;2E32]
   * - direction can be 0 or 1.
   */
  MyMotor.setBLDCspeed(speed, direction);
  MyMotor.startBLDCM();
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
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
    if(in == 'a'){
      MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);
      Serial.println(F("Motor stopped"));}
    if(in == 'q'){
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