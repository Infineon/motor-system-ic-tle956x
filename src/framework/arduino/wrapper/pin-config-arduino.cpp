/** 
 * @file        pin-config-arduino.cpp
 * @brief       Pin assignment for Arduino UNO
 * @copyright   2022 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pin-config-arduino.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)
#include <Arduino.h>

	#ifdef ARDUINO_AVR_UNO                                                  /** For Arduino UNO */ 

		hardwareconfig_t ARDUINO_UNO
		{
			.INTN = 2,                      //Interrupt input / Test

			.PWM_U = 3,                     //PWM1
			.PWM_V = 9,                     //PWM3
			.PWM_W = 10,                    //PWM5

			.BEMF_U_IO = 5,                 //BEMF input from comparator U
			.BEMF_V_IO = 6,                 //BEMF input from comparator V
			.BEMF_W_IO = 7,                 //BEMF input from comparator W

			.CSN = 8,                       //SPI Ship Select Input
			.SDI = 11,                      //SPI Data output to TLE (=MOSI)
			.SDO = 12,                      //SPI Data input from TLE (=MISO)
			.CLK = 13,                      //SPI Clock output

			.CSO = A5,                      //Current sense amplifier input

			.HALL_A = A2,                   //Hall sensor input
			.HALL_B = A1,                   //Hall sensor input
			.HALL_C = A0                    //Hall sensor input
		};

	#elif ((XMC1100_Boot_Kit)  || (XMC4700_Relax_Kit))                /** For XMC boards*/ 
		hardwareconfig_t ARDUINO_UNO
		{
			.INTN = 2,                      //Interrupt input / Test

			.PWM_U = 3,                     //PWM1
			.PWM_V = 9,                     //PWM3
			.PWM_W = 10,                    //PWM5

			.BEMF_U_IO = 5,                 //BEMF input from comparator U
			.BEMF_V_IO = 6,                 //BEMF input from comparator V
			.BEMF_W_IO = 7,                 //BEMF input from comparator W

			.CSN = 8,                       //SPI Ship Select Input
			.SDI = 11,                      //SPI Data output to TLE (=MOSI)
			.SDO = 12,                      //SPI Data input from TLE (=MISO)
			.CLK = 13,                      //SPI Clock output

			.CSO = 22,                      //Current sense amplifier input

			.HALL_A = 19,                   //Hall sensor input
			.HALL_B = 18,                   //Hall sensor input
			.HALL_C = 17                    //Hall sensor input
		};

	#endif
#endif /** MOTOR_SYSTEM_IC_FRAMEWORK **/