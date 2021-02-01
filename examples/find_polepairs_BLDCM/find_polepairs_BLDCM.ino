/*!
 * \name        find_polepairs_BLDCM.ino
 * \author      Infineon Technologies AG
 * \copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This example lets you BLDC motor do single steps in order to count, how many steps are necessary for one full revolution.
 * Thus it calculates, how many magnetic polepairs your motor has. A serial monitor is necessary.
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include "BLDCM-control-ino.hpp"
#include <SPI.h>

#define HALLSENSOR                  1     /* 1 = Hallsensor used, 0 = no hallsensor used */
#define DELAY_BETWEEN_COMMUTATIONS  300   /* milliseconds */

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  Serial.println(" Infineon TLE9563 BLDC shield Testsketch");
  Serial.println(" Mode: find polepairs");

  MyMotor.begin();
  MyMotor.setLED(0,0,50);      // Set onboard RGB-LED to low-bright blue.
  
  Serial.println("Init ready");
}

void loop()
{
	MyMotor.FindPolepairs(DELAY_BETWEEN_COMMUTATIONS, HALLSENSOR);
}