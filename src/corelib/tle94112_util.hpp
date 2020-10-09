/*!
 * \file        tle9563_util.hpp
 * \name        tle9563_util.hpp - automatically included configuration library
 * \author      Infineon Technologies AG
 * \copyright   2019 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This file can optionally be included in projects that use Infineon's 
 *              DC Motor Control Shield with TLE9563
 *              It provides a higher abstraction for controlling motors with the TLE9563 
 *              acting as an output driver
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_UTIL_HPP_
#define TLE9563_UTIL_HPP_

#include "TLE9563.hpp"

/**
 * @addtogroup tle9563utilapi
 * @{
 */

//! \brief SIGNUM helper function definition
#ifndef SIGNUM
#define SIGNUM(x) ( (x > 0) - (x < 0) )
#endif

//! \brief struct containing register locations for a single halfbridge
typedef struct
{
	uint8_t stateReg;
	uint8_t stateMask;
	uint8_t stateShift;
	uint8_t pwmReg;
	uint8_t pwmMask;
	uint8_t pwmShift;
	uint8_t fwReg;
	uint8_t fwMask;
	uint8_t fwShift;
	uint8_t ocReg;
	uint8_t ocMask;
	uint8_t ocShift;
	uint8_t olReg;
	uint8_t olMask;
	uint8_t olShift;
} HalfBridge_t;

//! \brief struct containing register locations for a single PWM channel
typedef struct 
{
	uint8_t freqReg;
	uint8_t freqMask;
	uint8_t freqShift;
	uint8_t dcReg;
	uint8_t dcMask;
	uint8_t dcShift;
} PWMchannel_t;
/** @} */

#endif /** end TLE9563_UTIL_HPP_ **/
