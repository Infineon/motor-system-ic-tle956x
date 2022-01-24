/*!
 * @name        TLE9562_DCM_rise-and-fall-time-regulation.ino
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG

 * @brief       This example lets you configure the rise- and falltime of a TLE9562. Therefore a closed-loop algorithm is implemented,
 * that prints the actual rise- /falltime and the gate charge- /dischargecurrent. Start the regulation by pressing 'u' in the serial monitor.
 * As soon as the values converge, stop the regulation by pressing 'j'. Note down the final charge- / dischargecurrent and edit the define-page in the library.
 *
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <DCM-control-ino.hpp>

#define HALFBRIDGE              PHASE1      // [PHASE1;Phase4] Select the phase on which you want to regulate Rise/Fall time
#define SPEED_INCREASE_STEP     100         // [1;511] speed step increase/decrease when pressing a key
#define CONTROL_LOOP_DELAY      400         // [ms] time between regulation executions

uint16_t speed = 400;                       // start speed
uint8_t direction = 0;                      // direction can not be changed in this example as it's determined by HALFBRIDGE

uint8_t trise_tg = 11;                      // [0;63] Initial Risetime target. Can be changed via keyboard input.
uint8_t tfall_tg = 11;                      // [0;63] Initial Falltime target. Can be changed via keyboard input.

uint8_t tRise, tFall, iCharge, iDischarge = 0;
uint32_t blinktimer = millis();             // Initialize rftreg_timer for the LED indicating Rise-/Falltime example code is running
bool ledstatus = 0;
bool riseFallTimeReg_enable = 0;
bool turnOnOffDelayReg_enable = 0;

// Create an instance of DCMcontrol called 'MyMotor'. 
DCMcontrolIno MyMotor = DCMcontrolIno();

void setup()
{
  Serial.begin(250000);
  Serial.println(F(" Infineon TLE9562 Gate Driver Configuration tool"));

  // Enable GPIO interrupt for pin 2
  attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);          // Set up a GPIO interrupt routine for error handling

  MyMotor.begin();
  MyMotor.configDCshield(AGC_ACTIVE);

  /**
   * setLED(led1, led2)
   * both values are 10-bit [0;1023]
   */
  MyMotor.setLED(0,100);                                                 // Switch on LED 2
  MyMotor.setupRiseFallTimeRegulation(HALFBRIDGE);
  MyMotor.setTrisefallTarget(trise_tg, tfall_tg);

  /**
   * setDCspeed(speed, direction, motoroutput)
   * - speed is the dutycycle speed in 10-bit, values between [0;1023]
   * - direction can be 0 or 1
   * - motoroutput is the motor you want to control.
   *    1 (= 0b01): motor connected to PHASE1 and PHASE2
   *    2 (= 0b10): motor connected to PHASE3 and PHASE4
   *    3 (= 0b11): both motors
   */
  MyMotor.setDCspeed(speed, direction, 3);
}

void loop()
{
    if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();

    if(in == '+')
    {
       speed += SPEED_INCREASE_STEP;
       Serial.print(F("Dutycycle: "));
       Serial.println(speed);
    }
    if(in == '-')
    {
      speed -= SPEED_INCREASE_STEP;
      Serial.print(F("Dutycycle: "));
      Serial.println(speed);
    }
    MyMotor.setDCspeed(speed, direction, 3);

    //==================== Adaptive Gate Charge control ======================
    if(in == 'u')
    {
      riseFallTimeReg_enable = 1;
      Serial.println(F("Rise- Fall-time regulation enabled"));
      Serial.println(F("iChg:\t tRise:\t iDchg:\t tFall:"));
    }
    if(in == 'j')
    {
      riseFallTimeReg_enable = 0;
      Serial.println(F("Rise- Fall-time regulation disabled"));
    }

    if(in == 'r')    // Increase target risetime
    {
      trise_tg += 1;
      Serial.print(F("tRise target: "));
      Serial.println(trise_tg);
      MyMotor.setTrisefallTarget(trise_tg, tfall_tg);
    }
    if(in == 'f')    // Decrease target risetime
    {
      trise_tg -= 1;
      Serial.print(F("tRise target: "));
      Serial.println(trise_tg);
      MyMotor.setTrisefallTarget(trise_tg, tfall_tg);
    }
    if(in == 't')    // Increase target falltime
    {
      tfall_tg += 1;
      Serial.print(F("tFall target: "));
      Serial.println(tfall_tg);
      MyMotor.setTrisefallTarget(trise_tg, tfall_tg);
    }
    if(in == 'g')    // Decrease target falltime
    {
      tfall_tg -= 1;
      Serial.print(F("tFall target: "));
      Serial.println(tfall_tg);
      MyMotor.setTrisefallTarget(trise_tg, tfall_tg);
    }

    //==================== Adaptive Gate Pre-charge control ======================
    if(in == 'i')
    {
      turnOnOffDelayReg_enable = 1;
      MyMotor.configDCshield(AGC_ACTIVE);
      Serial.println(F("Turn-on / -off delay regulation enabled"));
    }
    if(in == 'k')
    {
      turnOnOffDelayReg_enable = 0;
      MyMotor.configDCshield(AGC_INACTIVE1);
      Serial.println(F("Turn-on / -off delay regulation disabled"));
    }

    //MyMotor.setTrisefallTarget(trise_tg, tfall_tg);

  }

  if(MyMotor.checkTLEshield() > 0 )            // Check, if interrupt flag was set and read status register of TLE
  {
    MyMotor.setLED(100,0);                     // Switch on LED 1
  }

  if(riseFallTimeReg_enable)
  {
    MyMotor.riseFallTimeRegulation(HALFBRIDGE, &iCharge, &iDischarge, &tRise, &tFall);
    Serial.print(iCharge);
    Serial.print(F("\t "));
    Serial.print(tRise);
    Serial.print(F("\t "));
    Serial.print(iDischarge);
    Serial.print(F("\t "));
    Serial.println(tFall);
  }

  blinkLED();
  delay(CONTROL_LOOP_DELAY);

}

void TLEinterrupt()
{
  MyMotor.interrupt_status_changed = 1;
}

void blinkLED()
{
    if((millis() - blinktimer) > 500)
    {
        ledstatus = !ledstatus;
        MyMotor.setLED(0,ledstatus * 1000);
        blinktimer = millis();
    }
}