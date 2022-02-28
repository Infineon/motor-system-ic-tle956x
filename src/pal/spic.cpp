/** 
 * @file        spic.cpp
 * @brief       PAL of the SPI cover
 * @copyright   2022 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "spic.hpp"


SPIC::Error_t SPIC::checkErrorStatus()
{
	return errorStatus;
}