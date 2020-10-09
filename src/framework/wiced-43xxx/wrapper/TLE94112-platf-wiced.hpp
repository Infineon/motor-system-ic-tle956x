/**
 * @file        TLE9563-platf-wiced.hpp
 * @brief       TLE9563-platf-wiced.hpp Cypress WICED-43xxx Hardware Platforms
 * @date        Mai 2020
 * @version     2.0.0
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_PLATF_WICED_HPP_
#define TLE9563_PLATF_WICED_HPP_

#include "../pal/TLE9563-pal-wiced.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

#include <wiced_platform.h>

/**
 * @addtogroup wicedHW
 * @{
 */

/*!
 * Standard chip select pin for first TLE9563 shield
 */
#define TLE9563_PIN_CS1     WICED_GPIO_7

/*!
 * Standard chip select pin for second TLE9563 shield
 * To use a second shield with different CS pin you have
 * to remove the 0 Ohm resistor R7 and solder it to R8
 */
#define TLE9563_PIN_CS2     WICED_GPIO_17

/*!
 * Standard TLE9563 enable pin
 */
#define TLE9563_PIN_EN      WICED_GPIO_36

class Tle9563Wiced: virtual public TLE9563Wiced
{
	public:

		//! \brief standard constructor with default pin assignment
		Tle9563Wiced(void);

		/*! \brief constructor with individual pin assignment
		 *
		 * \param bus a void pointer to the object representing the SPI class
		 * \param csPin  pin number of the CS pin
		 */
		Tle9563Wiced(void* bus, uint8_t csPin);

		//! \brief enables and initializes the TLE9563
		void begin(void);

		/*! \brief constructor with individual pin assignment
		 *
		 * \param csPin  pin number of the CS pin
		 */
		void begin(uint8_t csPin);

		//! \brief deactivates all outputs and disables the TLE9563
		void end(void);

};
/**
 * @}
 */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563_PLATF_WICED_HPP_ **/
