/**
 * @file        timer-arduino.hpp
 * @brief       Arduino Timer PAL
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TIMER_ARDUINO_HPP_
#define TIMER_ARDUINO_HPP_

#include "../../../config/tle9xxx-conf.hpp"

#if (MOTOR_SYSTEM_IC_FRAMEWORK == TLE9XXX_FRMWK_ARDUINO)

#include "../../../pal/timer.hpp"

/**
 * @addtogroup arduinoPal
 * @{
 */

class TimerIno: virtual public Timer
{
	public:
		TimerIno();
		~TimerIno();
		Error_t init();
		Error_t deinit();
		Error_t start();
		Error_t elapsed(uint32_t &elapsed);
		Error_t stop();
		Error_t delayMilli(uint32_t timeout);
		Error_t delayMicro(uint32_t timeout);

	private:
		uint32_t startTime;
};
/** @} */

#endif /** MOTOR_SYSTEM_IC_FRAMEWORK **/
#endif /** TIMER_ARDUINO_HPP_ **/