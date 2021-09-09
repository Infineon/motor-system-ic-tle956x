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

#include "TLE9563.hpp"

#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include "../TLE9563-ino.hpp"
#endif

// ================================== Defines ==================================================================================================
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
		 * @brief set speed by handing over the dutycycle to the global _DutyCycle variable
		 * 
		 * @param dutycycle speed of the BLDCM (8-bit)
		 */
		void					setDCspeed(uint32_t speed, bool direction, uint8_t motorNumber);

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
		 * @brief 
		 * 
		 */
        /**
		 * @brief generate an instance of a TLE9563 controller used on this board
		 * 
		 */
		Tle9563 				*controller;
    private:
        AnalogDigitalConverter	*pwmA;			//<! \brief PWM output for motor A
		AnalogDigitalConverter	*pwmB;			//<! \brief PWM output for motor B

        Timer    				*timer;     	//<! \brief local timer for delay settings

};

/** @} */
#endif /** DCMCONTROL_HPP_ **/
