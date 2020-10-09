/**
 * @file        TLE9563Motor-platf-wiced.hpp
 * @brief       TLE9563Motor-platf-wiced.hpp Cypress WICED-43xxx Hardware Platforms
 * @date        Mai 2020
 * @version     2.0.0
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563MOTOR_PLATF_WICED_HPP_
#define TLE9563MOTOR_PLATF_WICED_HPP_

#include "TLE9563-platf-wiced.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

#include "../../../corelib/TLE9563Motor.hpp"

/**
 * @addtogroup wicedHW
 * @{
 */

class Tle9563MotorWiced: virtual public Tle9563Motor
{
};
/**
 * @}
 */

#endif /** TLE9563_FRAMEWORK **/
#endif /** TLE9563MOTOR_PLATF_WICED_HPP_ **/
