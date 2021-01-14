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
#include "TLE9563.hpp"

// TODO: implement logger-PAL for Platform abstraction
#include <Arduino.h>

#define SINGLE_STEP_DC          30
#define DETAILED_ERROR_REPORT 	1

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
    timer->delayMilli(800);
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

void BLDCMcontrol::PrintErrorMessage(_ErrorMessages msg)
{
    switch(msg)
    {
        case PARAMETER_MISSING:
            Serial.println("====> Error: For this operation mode one or more motor parameter(s) is missing! <====");
            break;
        case PARAMETER_OUT_OF_RANGE:
            break;
        default:
            break;
    }
    setLED(50,0,0);      // Set onboard RGB-LED to red
}

void BLDCMcontrol::PrintTLEErrorMessage(uint8_t msg, uint16_t &RegAddress, uint16_t &RegContent)
{
    if(msg & controller->TLE_SPI_ERROR)
    {
        Serial.println("===> Error: CRC / SPI-Failure <===");
    }
    else if(msg & controller->TLE_LOAD_ERROR)
    {
        Serial.println("===> Error: Open-Load detected <===");
    }
    else if(msg & controller->TLE_UNDER_VOLTAGE)
    {
        Serial.println("===> Error: Undervoltage detected! Check your voltage supply <===");
    }
    else if(msg & controller->TLE_OVER_VOLTAGE)
    {
        Serial.println("===> Error: Overvoltage detected! Check your voltage supply <====");
    }
    else if(msg & controller->TLE_POWER_ON_RESET)
    {
        Serial.println("===> Power on reset detected! <===");
    }
    else if(msg & controller->TLE_TEMP_SHUTDOWN)
    {
        Serial.println("===> Error: Temperature shutdown <===");
    }
    else if(msg & controller->TLE_OVERCURRENT)
    {
        Serial.println("===> Error: Overcurrent detected! <===");
    }
    else if(msg & controller->TLE_SHORT_CIRCUIT)
    {
        Serial.println("===> Error: Short circuit detected! <===");
    }

    if(DETAILED_ERROR_REPORT)
    {
        Serial.print("Reg: 0x");
        //PrintBinary(16, (address>>16) & 0xFFFF);
        Serial.print(RegAddress, HEX);
        Serial.print("  Content: ");
        PrintBinary(16, RegContent);
        Serial.println("");
    }

    setLED(50,0,0);      // Set onboard RGB-LED to red
}