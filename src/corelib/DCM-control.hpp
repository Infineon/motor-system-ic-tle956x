/*!
 * \file        DCM-control.hpp
 * \name        DCM-control.hpp - Arduino library to control Infineon's DC Motor Control Shield with TLE9562
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic functions to control DC motors using an instance of TLE9562
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef DCMCONTROL_HPP_
#define DCMCONTROL_HPP_

#include "TLE9562.hpp"

#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

/*
#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)
#include "../TLE9563-ino.hpp"
#endif
*/
// ================================== Defines ==================================================================================================
/**
 * @brief Main swithces here, use with care. 
 * with ADAPTIVE_GATE_CONTROL_PRECHARGE you can switch on the internal regulation of the gate-precharge current
 * with ADAPTIVE_GATE_CONTROL_CHARGE you can switch on the external control loop of the gate-charge current, implemented in this library
 * 
 */
#define ADAPTIVE_GATE_CONTROL_PRECHARGE			0		// 0 = INACTIVE1; 1 = INACTIVE2; 2 = ACTIVE | Built in AGC
#define ADAPTIVE_GATE_CONTROL_CHARGE			0		// 0 = INACTIVE; 1 = ACTIVE	| External AGC, control loop can be find in the TLE9xxx.cpp


/* Braking modes */
#define BRAKEMODE_PASSIVE						0
#define BRAKEMODE_ACTIVE						1

enum _Halfbridges{
			HB1,
			HB2,
			HB3,
			HB4
		};
enum _Outputs{
			OUT_A,
			OUT_B
		};
// =============================================================================================================================================

/**
 * @brief contains functions to control various types of DC motors using a TL9562 SBC
 * 
 * With this class you are able to run up to two motors with variable speed.
 * 
 */
class DCMcontrol
{

	public:

		volatile uint8_t 		interrupt_status_changed = 0;

		

        DCMcontrol(void);
        ~DCMcontrol(void);

        /**
		 * @brief initalize GPIOs and PWM pins
		 * 
		 */
		void 					begin();

		/**
		 * @brief Deinitialize GPIOs and PWM pins
		 * 
		 */
		void 					end();

		/**
		 * @brief config the TLE9562 chip with customized settings
		 * needs to be called once before the motor will be started
		 * 
		 * @return uint8_t 
		 */
		uint8_t					configDCshield(void);

		/**
		 * @brief check if status information is available
		 * If information is available, it will be printed automatically by the frontend.
		 * @param request select the registers/information that you want to observe
		 * @return uint8_t Error return code / status report
		 */
		uint8_t					checkTLEshield();

		/**
		 * @brief set the speed and direction for either a single motor or both motors
		 * 
		 * @param speed sets the dutycycle of the related PWM channel. 8 bit value (0-255)
		 * @param direction direction of rotation. 0 or 1.
		 * @param motorNumber which motor should be affected. 1 is HB 1&2. 2 is HB 3&4. 3 are all HBs.
		 */
		void					setDCspeed(uint16_t speed, bool direction, uint8_t motorNumber);

		/**
		 * @brief switch on the halbridges with preset speed and direction
		 * 
		 */
		void					startDCM(void);

		/**
		 * @brief Stop a DC motor be either switching off all halfbridges or let the float (TODO)
		 * 
		 * @param brakemode BRAKEMODE_PASSIVE or BRAKEMODE_ACTIVE
		 * @return uint8_t returns success or failure of stopping
		 */
		uint8_t					stopDCM(uint8_t brakemode);
		
		/**
		 * @brief set the intensity of the two onboard LEDs using two HSS on the TLE.
		 * 
		 * @param led1 brightness of LED D6 (10-bit)
		 * @param led2 brightness of LED D7 (10-bit)
		 */
		void 					setLED(uint16_t led1, uint16_t led2);

		/**
		 * @brief Reads out the fall time and rise time of the selected MOSFETS and prints the value(s) in nanoseconds to the serial monitor
		 * 
		 * @param hb which halfbridges should be read (4-bit)
		 */
		//void 					print_TFALL_TRISE(uint8_t hb);

		void					setupRiseFallTimeRegulation(uint8_t hb);
		
		/**
		 * @brief reads out the actual MOSFET rise-time (fall-time) and compares it to the desired rise-(fall-)time.
		 * The algorithm then adjusts the charge current ICHG for the active MOSFET of the selected halfbridge.
		 * 
		 * @param hb on which halfbridge should the algorithm be applied. Must be the same halfbridge where the PWM is routed to.
		 * @param risetime hands over the actual rise-time
		 * @param falltime hands oder the actual fall-time
		 */
		void					riseFallTimeRegulation(uint8_t hb, uint8_t &risetime, uint8_t &falltime);

        /**
		 * @brief generate an instance of a TLE9563 controller used on this board
		 * 
		 */
		Tle9562 				*controller;
    protected:
        AnalogDigitalConverter	*pwmA;			//<! \brief PWM output for motor A
		AnalogDigitalConverter	*pwmB;			//<! \brief PWM output for motor B

        Timer    				*timer;     	//<! \brief local timer for delay settings

		uint8_t					_DutyCycle = 0;
		uint8_t					_MotorStartEnable = 0;
		uint8_t					_Direction[2] = {0, 0};
		HBconfig_t				_HBstatus[4] = 	{controller->ActiveGround, controller->ActiveGround, controller->ActiveGround, controller->ActiveGround};

};

/** @} */
#endif /** DCMCONTROL_HPP_ **/
