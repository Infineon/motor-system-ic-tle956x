/*!
 * \file        TLE9563-ino.hpp
 * \name        TLE9563-ino.hpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \ref         tle9563ino
 * \version     2.0.0
 * \brief       This is a Arduino platform file use Infineon's DC Motor Control Shield with TLE9563 with
 *              Arduino/Genuino, Infineon XMC microcontroller or compatible boards.
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_INO_HPP_
#define TLE9563_INO_HPP_

#include "config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/**
 * @addtogroup inoApi
 * @{
 */

#include "corelib/TLE9563.hpp"
#include "framework/arduino/wrapper/TLE9563-platf-ino.hpp"

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

class Tle9563Ino: virtual public TLE9563Ino
{
	public:
		//! \brief standard constructor with default pin assignment
		Tle9563Ino(void);

		/*!
		 * \brief constructor with individual pin assignment
		 *
		 * \param bus    void pointer to the object representing the SPI class
		 * \param csPin  pin number of the CS pin
		 */
		Tle9563Ino(SPIClass &bus, uint8_t csPin);

		//! \brief enables and initializes the TLE9563
		void begin(void);

		//! \brief deactivates all outputs and disables the TLE9563
		void end(void);

};
/* @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_INO_HPP_ **/
