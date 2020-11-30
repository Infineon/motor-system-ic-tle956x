/*!
 * \name        BLDC_BEMF_test.ino
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This example runs a senorless brushlessmotor in BEMF mode using a TLE9563 BLDC control shield.
 * This file is only for debug purpose.
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include "BLDCM-control-ino.hpp"
#include <SPI.h>

#define DIRECTION	0

uint8_t DutyCycle = 80;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(" Infineon TLE9563 BLDC shield Testsketch");
  Serial.print(" Mode: ");
  Serial.println("find polepairs");

  MyMotor.begin();
  MyMotor.setLED(0,20,0);      // Set onboard RGB-LED to low-bright green.
  
  Serial.println("Init ready");
}

void loop()
{
	MyMotor.FindPolepairs(500, 1);
}