/**
 * @file        gpio-wiced.hpp
 * @brief       WICED PAL for the GPIO
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */


#ifndef GPIO_WICED_HPP_
#define GPIO_WICED_HPP_

#include "../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

#include "../../../pal/gpio.hpp"
#include <wiced_platform.h>

/**
 * @addtogroup wicedPal
 * @{
 */

class GPIOWiced : virtual public GPIO
{
	private:
		#define             UNUSED_PIN (wiced_gpio_t)(WICED_GPIO_51 + 1) /**< Unused pin */
		wiced_gpio_t        pin;                                         /**< GPIO number */
		wiced_gpio_config_t config;                                      /**< GPIO configuration */
		VLogic_t            logic;                                       /**< Pin logic */

	public:
					GPIOWiced();
					GPIOWiced(wiced_gpio_t pin, wiced_gpio_config_t config, VLogic_t logic);
					~GPIOWiced();
		Error_t     init();
		Error_t     deinit();
		VLevel_t    read();
		Error_t     write(VLevel_t level);
		Error_t     enable();
		Error_t     disable();
};


/** @} */

#endif /** TLE9563_FRAMEWORK **/
#endif /** GPIO_WICED_HPP_ **/
