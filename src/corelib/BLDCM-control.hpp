/*!
 * \file        BLDCM-control.hpp
 * \name        BLDCM-control.hpp - basic motor control functions
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
#include "Arduino.h"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include "../TLE9563-ino.hpp"
#endif

class BLDCMcontrol
{

	public:

		BLDCMcontrol(void);
		~BLDCMcontrol(void);

		void		begin(void);
		void		end(void);

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
		 * @brief set speed of a sensorless BLDC motor in BEMF mode using block commutation
		 * 
		 * @param dutycycle speed of the BLDCM (8-bit)
		 */
		void		setBLDCspeed(uint8_t dutycycle);

	protected:
		TLE9563Ino 	controller = TLE9563Ino();
		void		DoBEMFCommutation(void);
		void		UpdateHardware(uint8_t CommutationStep);

		uint8_t		_DutyCycle;
		uint8_t		_LastBLDCspeed = 0;
		uint8_t		_Commutation;
};

/** @} */
#endif /** BLDCMCONTROL_HPP_ **/