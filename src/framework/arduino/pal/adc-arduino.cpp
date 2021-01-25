/** 
 * @file        adc-arduino.cpp
 * @brief       Arduino PAL for the ADC
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */


#include "adc-arduino.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include <Arduino.h>
/**
 * @brief Default constructor of the Arduino ADC class
 * 
 */
ADCIno::ADCIno() : pin(0)
{

}

/**
 * @brief Constructor of the Arduino ADC class
 * 
 * This constructor takes the number of the pin.
 * 
 * @param[in]   pin Number of the desired ADC pin 
 */
ADCIno::ADCIno(uint8_t pin) : pin(pin)
{
    
}

/**
 * @brief Default constructor of the Arduino ADC
 * 
 */
ADCIno::~ADCIno()
{

}

/**
 * @brief ADC initialization
 * 
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::init()
{
    return OK;
}

/**
 * @brief ADC deinitialization
 * 
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::deinit()
{
    return OK;
}

/**
 * @brief ADC enable
 * 
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::enable()
{
    return OK;
}


/**
 * @brief ADC disable
 * 
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::disable()
{
    return OK;
}

/**
 * @brief Set resolution of the ADC
 * 
 * This function sets the resolution of the ADC.
 * The available resolutions are depending on the ADC of the device
 * and have be looked up in the datasheet.
 * The default resolution is set to 12 Bit, see constructor.
 * 
 * @param[in]   resolution  Value of the read resolution, default = 10 Bit 
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::setReadResolution(uint8_t resolution)
{
    // analogReadResolution(resolution);
    
    return OK;
}

/**
 * @brief Set the write resolution of the ADC
 * 
 * This function sets the write resolution of the ADC.
 * 
 * @param[in]   resolution  Value of the write resolution of the ADC
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::setWriteResolution(uint8_t resolution)
{
    // analogWriteResolution(resolution);
    
    return OK;
}

/**
 * @brief Set the write frequency of the ADC
 * 
 * This function sets the write resolution of the ADC.
 * 
 * @param[in]   divisor  Value of the divisor, frequency will be calculated according to the following description.
 * @return ADCIno::Error_t 
 * 
 *    Divides a given PWM pin frequency by a divisor.

   The resulting frequency is equal to the base frequency divided by
   the given divisor:
     - Base frequencies:
        o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
        o The base frequency for pins 5 and 6 is 62500 Hz.
     - Divisors:
        o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
          256, and 1024.
        o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
          128, 256, and 1024.

   PWM frequencies are tied together in pairs of pins. If one in a
   pair is changed, the other is also changed to match:
     - Pins 5 and 6 are paired on timer0
     - Pins 9 and 10 are paired on timer1
     - Pins 3 and 11 are paired on timer2

   Note that this function will have side effects on anything else
   that uses timers:
     - Changes on pins 3, 5, 6, or 11 may cause the delay() and
       millis() functions to stop working. Other timing-related
       functions may also be affected.
     - Changes on pins 9 or 10 will cause the Servo library to function
       incorrectly.

   Thanks to macegr of the Arduino forums for his documentation of the
   PWM frequency divisors. His post can be viewed at:
     https://forum.arduino.cc/index.php?topic=16612#msg121031
 */
ADCIno::Error_t ADCIno::setWriteFrequency(uint32_t frequency)
{
  uint8_t mode, divisor;
  divisor = frequency;    // abuse the frequency parameter to make the handling easier at the moment
/*
  if(30000 < frequency) divisor = 1;
  else if (15000 < frequency <= 30000) divisor = 2;
*/

#ifdef ARDUINO_AVR_UNO                                                  /** For Arduino UNO */
    if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
        switch (divisor) {
        case 1: mode = 0x01; break;
        case 8: mode = 0x02; break;
        case 64: mode = 0x03; break;
        case 256: mode = 0x04; break;
        case 1024: mode = 0x05; break;
        default: return;
        }
        if (pin == 5 || pin == 6) {
        TCCR0B = TCCR0B & 0b11111000 | mode;
        } else {
        TCCR1B = TCCR1B & 0b11111000 | mode;
        }
    } else if (pin == 3 || pin == 11) {
        switch (divisor) {
        case 1: mode = 0x01; break;
        case 8: mode = 0x02; break;
        case 32: mode = 0x03; break;
        case 64: mode = 0x04; break;
        case 128: mode = 0x05; break;
        case 256: mode = 0x06; break;
        case 1024: mode = 0x07; break;
        default: return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
    return OK;

#elif ARDUINO_AVR_MEGA2560                                                 /** For Arduino MEga 2560 */
    if (pin < 9 || pin > 10) {
      switch (divisor) {
      case 1: mode = 0x01; break;     // 31372,55 Hz  (62500,00 Hz on pin D4 & D13)
      case 8: mode = 0x02; break;     // 3921,16 Hz   (7812,50 Hz on pin D4 & D13)
      case 64: mode = 0x03; break;    // 490,20 Hz    (976,56 Hz on pin D4 & D13)
      case 256: mode = 0x04; break;   // 122,55 Hz    (244,14 Hz on pin D4 & D13)
      case 1024: mode = 0x05; break;  // 30,64 Hz     (61,04 Hz on pin D4 & D13)
      default: return;
      }
      if (pin == 4 || pin == 13) {
        TCCR0B = TCCR0B & 0b11111000 | mode;
      }
      else if (pin == 11 || pin == 12){
        TCCR1B = TCCR1B & 0b11111000 | mode;
      }
      else if (pin == 2 || pin == 3 || pin == 5){
        TCCR3B = TCCR3B & 0b11111000 | mode;
      }
      else if (pin == 6 || pin == 7 || pin == 8){
        TCCR4B = TCCR4B & 0b11111000 | mode;
      }
      else{
         TCCR5B = TCCR5B & 0b11111000 | mode;
      }
    }
    else if (pin == 9 || pin == 10) {
        switch (divisor) {
        case 1: mode = 0x01; break;     // 31372,55 Hz
        case 8: mode = 0x02; break;     // 3921,16 Hz
        case 32: mode = 0x03; break;    // 980,39 Hz
        case 64: mode = 0x04; break;    // 490,20 Hz
        case 128: mode = 0x05; break;   // 245,10 Hz
        case 256: mode = 0x06; break;   // 122,55 Hz
        case 1024: mode = 0x07; break;  // 30,64 Hz
        default: return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
    return OK;

#endif

}

/**
 * @brief Read the current ADC-Value
 * 
 * @return uint16_t ADC value 
 */
uint16_t ADCIno::ADCRead()
{
    uint16_t result = 0;

    result = analogRead(pin);

    return result;
}

/**
 * @brief Wirte on an analog pin
 * 
 * This functions writes the given value to the analog pin
 * 
 * @param[in] value PWM dutycycle to write
 * @return ADCIno::Error_t 
 */
ADCIno::Error_t ADCIno::ADCWrite(uint8_t value)
{
    analogWrite(pin, value);
    
    return OK;
}

#endif /** TLE9563_FRAMEWORK **/