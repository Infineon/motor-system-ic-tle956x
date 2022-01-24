/*!
 * @name        TLE9563_BLDCM_find-polepairs.ino
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This example lets you BLDC motor do single steps in order to count, how many steps are necessary for one full revolution.
 * Thus it calculates, how many magnetic polepairs your motor has. A serial monitor is necessary.
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include "BLDCM-control-ino.hpp"

#define HALLSENSOR                  1     /* 1 = Hallsensor used, 0 = no hallsensor used */
#define DELAY_BETWEEN_COMMUTATIONS  300   /* milliseconds */
#define DUTYCYCLE_SINGLE_STEP       30		/* dutycycle for single stepping */

// Create an instance of BLDCMcontrol called 'MyMotor'. 
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(250000);
  Serial.println(F(" Infineon TLE9563 BLDC shield Testsketch"));
  Serial.println(F(" Mode: find polepairs"));

  MyMotor.begin();
  MyMotor.setLED(0,0,200);      // Set onboard RGB-LED to blue.
}

void loop()
{
  uint8_t Hallpattern = 0;
	uint8_t Counter = 0;
  uint8_t CommutationStep = 0;
	uint8_t Magnetpoles = 0;
	uint8_t Magnetpolepairs = 0;
	uint8_t in;

	Serial.println(F("Mark a point at the rotation axis of your motor in order to determine its position."));
    delay(1000);

    Serial.println(F("Press enter to bring motor in start position"));
    while(Serial.available() == 0);
    in = Serial.read();
    Hallpattern = MyMotor.commutateHallBLDC(DUTYCYCLE_SINGLE_STEP, CommutationStep, HALLSENSOR);     //go in initial position
    delay(800);
    MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);

    Serial.println(F(" "));
    Serial.println(F("Press enter to start the measurement."));
    Serial.println(F("Press enter again to stop the measurement, when the motor did one full revolution"));
    Serial.println(F(" "));
    while(Serial.available() == 0);
    in = Serial.read();

	  Serial.println(F("Step | Commutation | HallpatternDEC | HallpatternBIN"));
    while(Serial.available() == 0)
    {
        Counter ++;
        CommutationStep ++;
        if(CommutationStep == 6) CommutationStep = 0;       // Make sure the CommutationStep is a value between 0 and 5
        Hallpattern = MyMotor.commutateHallBLDC(DUTYCYCLE_SINGLE_STEP, CommutationStep, HALLSENSOR);
        Serial.print(Counter);
        Serial.print(F("\t "));
        Serial.print(CommutationStep);
        Serial.print(F("\t\t "));	
        if(HALLSENSOR)
        {
            Serial.print(Hallpattern);				            // Print hall pattern decimal
            Serial.print(F("\t\t "));
			      printBinary(3, Hallpattern);			            // Print hall pattern binary
			      Serial.println(F(""));
        }
        else Serial.println(F(" /"));
        delay(DELAY_BETWEEN_COMMUTATIONS);
    }
    in = Serial.read();			// empty serial buffer

	MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);

	// Evaluation
    if((Counter % 2) == 1)
    {
        Serial.println(F("Please try again, 'Step' must be a even number, when you stop the motor"));
    }
    else if((Counter % 6) > 0)
    {
        Serial.println(F("Please try again, 'Step' must be a multiple of 6"));
    }
    else
    {
        Magnetpolepairs = Counter/6;
        Magnetpoles = Magnetpolepairs * 2;
        Serial.print(F("Your motor has "));
        Serial.print(Magnetpolepairs);
        Serial.print(F(" polepairs (equal to "));
        Serial.print(Magnetpoles);
        Serial.println(F(" poles)"));
    }
    Serial.println(F("======================================================================"));
    Serial.println(F(""));

	delay(3000);
  Counter = 0;
}

void printBinary(uint8_t digits, uint16_t number)
{
  for(uint8_t i=digits-1; i>0; i--)
  {
    if(pow(2, i) <= number)
    {
      break;
    }
    Serial.print(F("0"));
  }
  Serial.print(number,BIN);
}