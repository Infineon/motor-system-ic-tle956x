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

uint8_t DutyCycle = 80;
uint16_t StatusReg[2];
uint8_t InterruptStatus = 0;
uint8_t InterruptEnable = 0;
uint8_t Outputcounter=0;
uint8_t CommStat = 0;
uint8_t CommStatPrev = 0;

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  MyMotor.begin();
  MyMotor.setLED(20,0,0);      // Set onboard RGB-LED to low-bright red (indicates motor is stopped)
  attachInterrupt(digitalPinToInterrupt(2), InterruptHandler, FALLING);
  
  Serial.println("Init ready");
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+') DutyCycle += 2;          // Adapt the speed with keyboard input in the serial monitor
    if(in == '-') DutyCycle -= 2;
    if(in == 'i') InterruptEnable = 1;
    if(in == 'k') InterruptEnable = 0;
    Serial.println(DutyCycle);
  }
  MyMotor.setBLDCspeed(DutyCycle);
  CommStat = MyMotor.DoBEMFCommutation();

  if(CommStat == 2 && CommStat != CommStatPrev)
  {
    MyMotor.setLED(0,20,0);      // Set onboard RGB-LED to low-bright green (indicates Motor is running)
    CommStatPrev = CommStat;
  }

  Outputcounter++;
  if(Outputcounter > 20)
  {
    if(InterruptStatus==1 && InterruptEnable==1)
    {
      MyMotor.controller.updateStatus(StatusReg);
      printStatusReg(16, StatusReg[0]);
      Serial.print("\t");
      printStatusReg(4, StatusReg[1]);
      Serial.println("");
      InterruptStatus = 0;
    }
    Outputcounter = 0;
  }
}

/**
 * @brief fill up the missing '0's before a binary number
 * 
 * @param digits how many digits, 8? 16?
 * @param number The number you want to print
 */
void printStatusReg(uint8_t digits, uint16_t number)
{
  for(uint8_t i=digits-1; i>0; i--)
  {
    if(pow(2, i) <= number)
    {
      break;
    }
    Serial.print("0");
  }
  Serial.print(number,BIN);
}

/**
 * @brief called by attach interrupt
 * 
 */
void InterruptHandler(void)
{
  InterruptStatus =1;
}
// 1010010100001100
// 0000010001000000
