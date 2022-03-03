/**
 * @file        gpio.cpp
 * @brief       PAL of the GPIO
 * @copyright   2022 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "gpio.hpp"


GPIO::Error_t GPIO::checkErrorStatus()
{
	return errorStatus;
}