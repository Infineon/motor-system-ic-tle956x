/**
 * @file        gpio-arduino.hpp
 * @brief       Arduino PAL for the GPIO
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef GPIO_ARDUINO_HPP_
#define GPIO_ARDUINO_HPP_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

#include "../../../corelib/TLE9563.hpp"


/**
 * @addtogroup arduinoPal
 * @{
 */


/**
 * @brief Arduino GPIO class
 *
 */
class GPIOIno: virtual public GPIO
{
private:
	uint8_t     pin;
	uint8_t     mode;
	VLogic_t    logic;

public:
				GPIOIno();
				GPIOIno(uint8_t pin, uint8_t mode, VLogic_t logic);
				~GPIOIno();
	Error_t     init();
	Error_t     deinit();
	VLevel_t    read();
	Error_t     write(VLevel_t level);
	Error_t     enable();
	Error_t     disable();
};
/** @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** GPIO_ARDUINO_HPP_ **/
