/*!
 * @file        TLE9563.hpp
 * @name        TLE9563.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic common functions to communicate with a TLE9563 BLDC controller
 * @ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_HPP_
#define TLE9563_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "TLE9xxx.hpp"

// ================================== Defines ==================================================================================================
#define PWM_CTRL_DC_MASK			0x3FF0
#define PWM_BNK_MODULE_1			0x0
#define PWM_BNK_MODULE_2			0x1
#define PWM_BNK_MODULE_3			0x2

/****************** Current sense amplifier *******************/
#define CSA_PWM_NB					0
#define CSA_CSO_CAP					1
#define CSA_CSD						0			// 0 = Unidirectional, 1 = Bidirectional
#define CSA_OCFILT					0b01		// [0;3] Filter time
#define CSA_OFF						1			// 0=enabled, 1=disabled

#define CONF_CSA_OCTH				0b11		// [0;3] Overcurrent detection threshold
#define CONF_CSA_CSAG				G_DIFF20	// [0;3]
#define CONF_CSA_OCEN				0			// Overcurrent shutdown. 0 = disabled, 1 = enabled

#define ADC_REF_VOLTAGE       		5.0       	// [v] Microcontroller reference voltage
#define ADC_RESOLUTION        		1024.0
#define CSA_VREF_UNIDIR				1.0			// [V], V_CC1/5
#define CSA_VREF_BIDIR				2.5			// [V], V_CC1/2

enum _CSA_Gains{
		G_DIFF10 = 0,
		G_DIFF20 = 1,
		G_DIFF40 = 2,
		G_DIFF60 = 3
	};
// ===============================================================================================================================================
/**
 * @addtogroup tle9563api
 * @{
 */

/**
 * @brief represents a TLE9563
 *
 * This class inherits all basig functions a Tle9xxx SBC offers and adds features, a TLE9563 BLDC controller is capable of.
 * This includes the triple gatedriver, mainly used for the MOSFETS of the three BLDCM phases.
 * Further three high side switches (hss) for various applications, but in this Lib they are intended
 * to drive an RGB LED like on the BLDC control board, using the TLE onboard PWM timer slices.
 */
class Tle9563: public Tle9xxx
{
	public:
		//! \brief standard constructor with default pin assignment
		Tle9563();

		//! \brief standard destructor switches shield off
		~Tle9563();

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
		 * @brief set up all necessary registers and initialize the TLE9563
		 * Set Adaptive gate control default to ACTIVE.
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
		 */
		void 					setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3);

		/**
		 * @brief control the high side switches
		 * The function assigns each highsideswitch a separate PWM slice of TLE9563.
		 * 
		 * @param hss1 dutycycle for highsideswitch 1 (10-bit)
		 * @param hss2 dutycycle for highsideswitch 1 (10-bit)
		 * @param hss3 dutycycle for highsideswitch 1 (10-bit)
		 */
		void 					setHSS(uint16_t hss1, uint16_t hss2, uint16_t hss3);

		/**
		 * @brief Set the General Bridge Control data
		 * Most Flags are taken from the defines in this header file.
		 */
		void					setGenControl(void);

		/**
		 * @brief calculates the Voltage across the CSA
		 * The maximum current that can be measured with G_DIFF20 is 49,8A.
		 * The resolution is 48,7mA.
		 * @return float returns the voltage in V
		 */
		float					getCSOVoltage(void);

		HBconfig_t 				ActiveGround; 
		HBconfig_t 				ActivePWM; 
		HBconfig_t 				Floating; 

		AnalogDigitalConverter	*cso;			//<! \brief Current sense amplifier input

	protected:
		/**
		 * @brief configure the current sense amplifier
		 * 
		 * @param octh overcurrent detection threshold of CSO
		 * @param csag Gain of the current sense amplifier
		 * @param ocen overcurrent shutdown enable
		 */
		void					setCSA(uint8_t octh, uint8_t csag, bool ocen);
		float 					csa_gain_table[4] = {10.04, 20.05, 40.05, 60.12};


};
/** @} */

#endif /** TLE9563_HPP_ **/
