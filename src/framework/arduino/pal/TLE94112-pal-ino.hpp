/*!
 * \file        TLE9563-pal-ino.hpp
 * \name        TLE9563-pal-ino.hpp - Arduino Hardware Abstraction Layer
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     2.0.0
 * \ref         arduinoPal
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_PAL_INO_HPP_
#define TLE9563_PAL_INO_HPP_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/**
 * @addtogroup arduinoPal
 * @{
 */

#include <Arduino.h>
#include <SPI.h>
#include "../../../corelib/TLE9563.hpp"
#include "gpio-arduino.hpp"
#include "timer-arduino.hpp"
#include "spic-arduino.hpp"

/**
 * @brief represents a basic TLE9563
 *
 * This class provides a simple API for connecting and controlling motors.
 * Each motor is assigned to a Tle9563 which acts as output driver. Calls to
 * Tle9563Motor instances are mapped to calls to Tle9563. Therefore, this
 * class does not bring new features, it does only provide further abstraction.
 *
 * @see Tle9563
 */
class TLE9563Ino: virtual public Tle9563
{
	protected:

		/*!
		 * \brief writes data bits to a control register of the TLE9563
		 *
		 * \param reg   control register number(mapping array index / CtrlRegisters constant) of the register
		 * \param mask  mask for the bits that have to be written
		 * \param shift data will be shifted left by this amount before masking. This is for the bit alignment of data
		 * \param data  the data byte that has to be written. It will be shifted and masked before
		 *
		 * \see CtrlRegisters
		 * \see TLE9563_NUM_CTRL_REGS
		 * \see mCtrlRegAddresses
		 * \see mCtrlRegData
		 */
		void writeReg(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t data);

		/*!
		 * \brief reads one byte from a status register of the TLE9563
		 *
		 * \param reg status register number(mapping array index / StatusRegisters constant) of the register
		 *
		 * \return data byte that has been read
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		uint8_t readStatusReg(uint8_t reg);

		/*!
		 * \brief reads some bits from a status register of the TLE9563
		 *
		 * \param reg   status register number(mapping array index / StatusRegisters constant) of the register
		 * \param mask  mask for the bits that have to be read
		 * \param shift data will be shifted right by this amount after masking. This is for the bit alignment of data
		 *
		 * \return data bits that have been read (after masking and shifting)
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		uint8_t readStatusReg(uint8_t reg, uint8_t mask, uint8_t shift);

		/*!
		 * \brief clears a status register by writing 0x00 to it
		 *
		 * \param reg status register number(mapping array index / StatusRegisters constant) of the register
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		void clearStatusReg(uint8_t reg);

};
/**
 * @}
 */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_PAL_INO_HPP_ **/