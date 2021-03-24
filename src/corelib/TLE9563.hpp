/*!
 * \file        TLE9563.hpp
 * \name        TLE9563.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic common functions to communicate with a TLE9563 BLDC controller
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_HPP_
#define TLE9563_HPP_

#include <stdint.h>
#include <stdlib.h>

#include "TLE9563-reg.hpp"
#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

/*! \brief SPI address commands */
#define TLE9563_CMD_WRITE          	0x80
#define TLE9563_CMD_READ          	0x00
#define TLE9563_CMD_CLEAR          	0x80

#define PWM_CTRL_DC_MASK			0x3FF0
#define PWM_BNK_MODULE_1			0x0
#define PWM_BNK_MODULE_2			0x1
#define PWM_BNK_MODULE_3			0x2

/**
 * @addtogroup tle9563api
 * @{
 */
	typedef struct
	{
		uint8_t HBmode;
		uint8_t Freewheeling;
		uint8_t PWMenable;
	} HBconfig_t;

/**
 * @brief represents a basic TLE9563
 *
 * This class provides all features, a TLE956x BLDC controller SBC is capable of.
 * This includes the triple gatedriver, mainly used for the MOSFETS of the three BLDCM phases.
 * Further three high side switches (hss) for various applications, but in this Lib they are intended
 * to drive an RGB LED like on the BLDC control board, using the TLE onboard PWM timer slices.
 */
class Tle9563
{
	public:
		//! \brief enum for the flags
		enum DiagFlag
		{
			TLE_SPI_ERROR = 0x80,
			TLE_LOAD_ERROR = 0x40,
			TLE_UNDER_VOLTAGE = 0x20,
			TLE_OVER_VOLTAGE = 0x10,
			TLE_POWER_ON_RESET = 0x08,
			TLE_TEMP_SHUTDOWN = 0x04,
			TLE_OVERCURRENT = 0x02,
			TLE_SHORT_CIRCUIT = 0x01
		};

		//! \brief standard constructor with default pin assignment
		Tle9563();

		//! \brief standard destructor switches shield off
		~Tle9563();

		/**
		 * @brief set up all necessary registers and initialize the TLE9563
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
		 * @brief set the Interrupt Mask to select which events should cause a hardware pin interrupt generation
		 * 
		 */
		void 					configInterruptMask(void);

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
		 * @brief read the Supply Voltage Fail Status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint8_t					checkStatSUP(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the Thermal Protection status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint8_t					checkStatTHERM(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the High-Side Switch status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint8_t					checkStatHSS(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the Device Information status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint8_t					checkStatDEV(uint16_t &RegAddress, uint16_t &RegContent);

		HBconfig_t 				ActiveGround; 
		HBconfig_t 				ActivePWM; 
		HBconfig_t 				Floating; 

		GPIO     				*intn;        	//<! \brief interrupt / test input
		AnalogDigitalConverter	*cso;			//<! \brief Current sense amplifier input
		
	protected:

		/**
		 * @brief disable the cyclic redundancy check of TLE9563
		 */
		void 					SBC_CRC_Disable();

		/**
		 * @brief send SPI comands to the TLE9563
		 * If CRC is disabled, static pattern will be added automatically.
		 * TODO: insert another layer below writeReg, so this function can be used for SPI as well as for CAN and Lin
		 * 
		 * @param addr addres of the register you want access (use the enum in TLE9563-reg.cpp)
		 * @param data data you want to transmitt (16-bit)
		 */
		void 					writeReg(uint8_t addr, uint16_t data);

		/**
		 * @brief read SPI commands from TLE9563
		 * 
		 * @param addr 		addres of the register you want to read from (use the enum in TLE9563-reg.cpp)
		 * @return uint16_t returned value of TLE9563 (16-bit)
		 */
		uint16_t 				readReg(uint8_t addr);


		SPIC     				*sBus;      	//<! \brief SPI cover class as representation of the SPI bus
		GPIO     				*csn;        	//<! \brief shield enable GPIO to switch chipselect on/of
		Timer    				*timer;     	//<! \brief timer for delay settings


};
/** @} */

#endif /** TLE9563_HPP_ **/
