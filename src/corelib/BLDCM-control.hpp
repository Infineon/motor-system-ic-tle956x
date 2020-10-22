/*!
 * \file        BLDCM-control.hpp
 * \name        BLDCM-control.hpp - basic motor control functions
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
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
		// Tle9563 Object on Shield 1
		BLDCMcontrol(void);
		~BLDCMcontrol(void);

		void		begin(void);
		void		end(void);
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