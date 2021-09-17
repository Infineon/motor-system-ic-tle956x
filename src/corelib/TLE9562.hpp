/*!
 * \file        TLE9562.hpp
 * \name        TLE9562.hpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This library includes the basic common functions to communicate with a TLE9562 motor controller
 * \ref         tle9562corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9562_HPP_
#define TLE9562_HPP_

#include <stdint.h>
#include <stdlib.h>

#include "TLE9xxx.hpp"

#define PWM_CTRL_DC_MASK			0x3FF0
#define PWM_BNK_MODULE_1			0x0
#define PWM_BNK_MODULE_2			0x1
#define PWM_BNK_MODULE_3			0x2

/**
 * @addtogroup tle9563api
 * @{
 */

/**
 * @brief represents a TLE9562
 *
 * This class inherits all basig functions a Tle9xxx SBC offers and adds features, a TLE9562 BLDC controller is capable of.
 * This includes the triple gatedriver, mainly used for the MOSFETS of the three BLDCM phases.
 * Further three high side switches (hss) for various applications, but in this Lib they are intended
 * to drive an RGB LED like on the BLDC control board, using the TLE onboard PWM timer slices.
 */
class Tle9562: public Tle9xxx
{
	public:
		//! \brief standard constructor with default pin assignment
		Tle9562();

		//! \brief standard destructor switches shield off
		~Tle9562();

		/**
		 * @brief initalize GPIOs and PWM pins
		 * 
		 */
		void 					begin();

		/**
		 * @brief Deinitalize GPIOs and PWM pins
		 * 
		 */
		void 					end();
		
		/**
		 * @brief set up all necessary registers and initialize the TLE9562
		 */
		void 					config(void);

		/**
		 * @brief Set the Halfbridge object
		 * This function is necessary to control the MOSFETS of a BLDC shield.
		 * Each halfbridge can be PASSIVE_OFF, LOWSIDE_ON, HIGHSIDE_ON or ACTIVE_OFF.
		 * Moreover it can be configured, if active freewheeling for each halfbridge should be turned on or off,
		 * as well as if an external PWM should be guided through.
		 * 
		 * @param hb1 struct element with parameters described above for halfbridge 1
		 * @param hb2 struct element with parameters described above for halfbridge 2
		 * @param hb3 struct element with parameters described above for halfbridge 3
         * @param hb4 struct element with parameters described above for halfbridge 4
		 */
		void 					setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3, HBconfig_t hb4);

		/**
		 * @brief control the high side switches
		 * The function assigns each highsideswitch a separate PWM slice of TLE9562.
		 * 
		 * @param hss1 dutycycle for highsideswitch 1 (10-bit)
		 * @param hss2 dutycycle for highsideswitch 2 (10-bit)
		 * @param hss3 dutycycle for highsideswitch 3 (10-bit)
		 * @param hss4 dutycycle for highsideswitch 4 (10-bit)
		 */
		void 					setHSS(uint16_t hss1, uint16_t hss2, uint16_t hss3, uint16_t hss4);

		HBconfig_t 				ActiveGround; 
		HBconfig_t 				ActivePWM; 
		HBconfig_t 				Floating; 
		
	protected:


};
/** @} */

#endif /** TLE9562_HPP_ **/
