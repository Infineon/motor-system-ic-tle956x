/*!
 * @file        TLE9562.hpp
 * @name        TLE9562.hpp - Arduino library to control Infineon's DC Motor Control Shield with Tle9562
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to communicate with a TLE9562 motor controller
 * @ref         tle9562corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9562_HPP_
#define TLE9562_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "TLE9xxx.hpp"

// ================================== Defines ==================================================================================================


// =============================================================================================================================================

/**
 * @addtogroup tle9562api
 * @{
 */

/**
 * @brief represents a TLE9562
 *
 * This class inherits all basic functions a Tle9xxx SBC offers and adds features, a TLE9562 BLDC controller is capable of.
 * This includes the four gatedrivers, mainly used for the MOSFETS of the two DC-motor outputs.
 * Further four high side switches (hss) for various applications can be controlled. In this Lib two of them are used to drive the two onboard red LEDs,
 * using the 10-bit TLE PWM timer slices.
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
		 * 
		 * @param agc switch the adaptive gate control for pre-charge current [0;2]
		 */
		void 					config(uint8_t agc = AGC_ACTIVE);

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

		/**
		 * @brief Set the General Bridge Control data
		 * Most Flags are taken from the defines in this header file, only PWM mapping is changeable.
		 * 
		 * @param MapPWM1 Map PWM channel 1 to a halfbridge. Use either the define 'PWM1_TO_HB1' or 'PWM1_TO_HB2'
		 * @param MapPWM2 Map PWM channel 3 to a halfbridge. Use either the define 'PWM3_TO_HB3' or 'PWM3_TO_HB4'
		 */
		void					setGenControl(bool MapPWM1, bool MapPWM2);

		HBconfig_t 				ActiveGround; 
		HBconfig_t 				ActivePWM; 
		HBconfig_t 				Floating; 
		
	protected:


};
/** @} */

#endif /** TLE9562_HPP_ **/
