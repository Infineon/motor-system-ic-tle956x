/*!
 * \file        BLDCM-control.hpp
 * \name        BLDCM-control.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BLDCMCONTROL_HPP_
#define BLDCMCONTROL_HPP_

#include "TLE9563.hpp"

#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include "../TLE9563-ino.hpp"
#endif

/**
 * @brief contains functions to control various types of BLDC motors using a TL9563 SBC
 * 
 * With this class you are able to run a BLDC motor in BEMF mode or hallsensor mode using block commutation.
 * 
 */
class BLDCMcontrol
{

	public:

		BLDCMcontrol(void);
		~BLDCMcontrol(void);

		/**
		 * @brief set color and brightness of the onboard RGB-LED
		 * 
		 * Uses the the highside switches and PWM modules of TLE9563.
		 * Each color has a 10-bit accuracy.
		 * @param red 		brightness of the red LED (10-bit)
		 * @param green 	brightness of the green LED (10-bit)
		 * @param blue 		brightness of the blue LED (10-bit)
		 */
		void 		setLED(uint16_t red, uint16_t green, uint16_t blue);

		/**
		 * @brief set speed by handing over the dutycycle to the global _DutyCycle variable
		 * 
		 * @param dutycycle speed of the BLDCM (8-bit)
		 */
		void		setBLDCspeed(uint8_t dutycycle);

		/**
		 * @brief do a blind commutation to start up the BLDC in BEMF mode
		 * 
		 */
		void		startBLDCM(void);

		/**
		 * @brief depending on the actual commutation state, wait for the next BEMF flag to commutate
		 * 
		 * @return uint8_t returns success or failure of startup / commutation
		 */
		uint8_t		DoBEMFCommutation(void);

		/**
		 * @brief initalize GPIOs and PWM pins
		 * 
		 */
		void 		begin();

		/**
		 * @brief Deinitialize GPIOs and PWM pins
		 * 
		 */
		void 		end();

		TLE9563Ino 	controller = TLE9563Ino();		// ! TODO: Move to BLDCM-control-ino.cpp file in order to keep platform abstraction ! (But how?)
		
	protected:

		

		AnalogDigitalConverter	*pwmU;			//<! \brief PWM output for phase U
		AnalogDigitalConverter	*pwmV;			//<! \brief PWM output for phase V
		AnalogDigitalConverter	*pwmW;			//<! \brief PWM output for phase W

		GPIO					*bemfU;			//<! \brief BEMF U input from analog comparator
		GPIO					*bemfV;			//<! \brief BEMF V input from analog comparator
		GPIO					*bemfW;			//<! \brief BEMF W input from analog comparator

		GPIO					*hallA;			//<! \brief Hall input A
		GPIO					*hallB;			//<! \brief Hall input B
		GPIO					*hallC;			//<! \brief Hall input C

		Timer    				*timer;     	//<! \brief timer for delay settings

		/**
		 * @brief contains the six steps of the six-step-blockcommutation
		 * in each step the gatedrivers of the TLE9563 are updated to either PWM, Ground or Floating.
		 * At the same time the PWM is assigned to the right output pin.
		 * 
		 * @param CommutationStep the actual commutation state, values 0 to 5 are allowed
		 */
		void		UpdateHardware(uint8_t CommutationStep);

		uint8_t		_DutyCycle;
		uint8_t		_LastBLDCspeed = 0;
		uint8_t		_Commutation;
};

/** @} */
#endif /** BLDCMCONTROL_HPP_ **/