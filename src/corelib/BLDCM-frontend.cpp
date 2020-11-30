/*!
 * \file        BLDCM-frontend.cpp
 * \name        BLDCM-frontend.cpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes functions to prepare and return debug and status information about the TLE9563 BLDC shield
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "BLDCM-control.hpp"

// TODO: implement logger-PAL for Platform abstraction
#include <Arduino.h>

#define SINGLE_STEP_DC 50

void BLDCMcontrol::PrintBinary(uint8_t digits, uint16_t number)
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

void BLDCMcontrol::FindPolepairs(uint16_t delay, bool hallsensor)
{
	uint8_t Hallpattern = 0;
	uint8_t Counter = 0;
	uint8_t Magnetpoles = 0;
	uint8_t Magnetpolepairs = 0;
	uint8_t in;

	Serial.println("Mark a point at the rotation axis of your motor in order to determine its position.");
    timer->delayMilli(1000);

    Serial.println("Press enter to bring motor in start position");
    while(Serial.available() == 0);
    in = Serial.read();
    Hallpattern = CommutateHallBLDC(SINGLE_STEP_DC, hallsensor);     //go in initial position
    timer->delayMilli(1000);
    StopBLDCM(PASSIVE);

    Serial.println(" ");
    Serial.println("Press enter to start the measurement.");
    Serial.println("Press enter again to stop the measurement, when the motor did one full revolution");
    Serial.println(" ");
    while(Serial.available() == 0);
    in = Serial.read();

	Serial.println("Step | Commutation | HallpatternDEC | HallpatternBIN");
    while(Serial.available() == 0)
    {
        Counter ++;
        Hallpattern = CommutateHallBLDC(SINGLE_STEP_DC, hallsensor);
        Serial.print(Counter);						// Print Step
        if(Counter < 10) Serial.print(" ");         //Align values
        Serial.print("          ");
        Serial.print(_Commutation);					// Print Commutation
        Serial.print("          ");	
        if(hallsensor == 1)
        {
            Serial.print(Hallpattern);				// Print hall pattern decimal
            Serial.print("          ");
			PrintBinary(3, Hallpattern);			// Print hall pattern binary
			Serial.println("");
        }
        else Serial.println(" / ");
        timer->delayMilli(delay);
    }
    in = Serial.read();			// empty serial buffer

	StopBLDCM(PASSIVE);

	// Evaluation
    if((Counter % 2) == 1)
    {
        Serial.println("Please try again, it must be a even number, when you stop the motor");
    }
    else if((Counter % 6) > 0)
    {
        Serial.println("Please try again, it must be a multiple of 6");
    }
    else
    {
        Magnetpolepairs = Counter/6;
        Magnetpoles = Magnetpolepairs * 2;
        Serial.print("Your motor has ");
        Serial.print(Magnetpolepairs);
        Serial.print(" polepairs (equal to ");
        Serial.print(Magnetpoles);
        Serial.println(" poles)");
    }
    Serial.println("======================================================================");
    Serial.println("");

	timer->delayMilli(3000);
    Counter = 0;
}