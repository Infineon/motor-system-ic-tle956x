/**
 * @file        spic-arduino.hpp
 * @brief       Arduino PAL for the SPI cover
 * @copyright   2022 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */


#ifndef SPIC_ARDUINO_HPP_
#define SPIC_ARDUINO_HPP_

#include "../../../config/tle9xxx-conf.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)

#include "../../../pal/spic.hpp"
#include <Arduino.h>
#include "SPI.h"

#include "timer-arduino.hpp"
#include "../../../corelib/TLE9563.hpp"
/**
 * @addtogroup arduinoPal
 * @{
 */


/**
 * @brief Arduino SPIC class
 *
 */
class SPICIno: virtual public SPIC
{
	private:
		uint8_t     csPin;
		uint8_t     misoPin;
		uint8_t     mosiPin;
		uint8_t     sckPin;
		SPIClass    *spi;
		uint8_t     lsb;
		uint8_t     mode;
		uint8_t     clock;

	public:
					SPICIno();
					SPICIno(uint8_t lsb, uint8_t mode, uint8_t clock);
					SPICIno(SPIClass &port, uint8_t csPin, uint8_t misoPin=MISO, uint8_t mosiPin=MOSI, uint8_t sckPin=SCK);
					~SPICIno();
		Error_t     init();
		Error_t     deinit();
		Error_t     transfer(uint8_t send, uint8_t &received);
		Error_t     transfer16(uint16_t send, uint16_t &received);
};
/** @} */

#endif /** MOTOR_SYSTEM_IC_FRAMEWORK **/
#endif /** SPIC_ARDUINO_HPP_ **/
