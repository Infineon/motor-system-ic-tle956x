/*!
 * \file        TLE9563-ino.hpp
 * \name        TLE9563-ino.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \ref         tle9563ino
 * \version     0.0.1
 * \brief       This is a Arduino platform file use Infineon's BLDC Motor Control Shield with TLE9563 with
 *              Arduino/Genuino, Infineon XMC microcontroller or compatible boards.
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_INO_HPP_
#define TLE9563_INO_HPP_

#include "config/tle9563-conf.hpp"
#include "corelib/TLE9563.hpp"


#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/**
 * @addtogroup inoApi
 * @{
 */
#include "framework/arduino/wrapper/pin-config-arduino.hpp"

/**
 * @class Tle9563Ino
 *
 * @brief represents the TLE9563 base class
 *
 * This class provides a simple API for connecting the TLE9563 shield via SPI.
 * The 12 halfbridges, the PWM units and the registers are set and controlled
 * from the library.
 *
 */
class TLE9563Ino: virtual public Tle9563
{
	public:
		//! \brief standard constructor with default pin assignment
		TLE9563Ino(void);

		//! \brief enables and initializes the TLE9563
		void begin(void);

		//! \brief deactivates all outputs and disables the TLE9563
		void end(void);

	protected:

		void SBC_CRC_Disable();
		void SBC_SPI(uint8_t addr, uint16_t data);
};
/* @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_INO_HPP_ **/
