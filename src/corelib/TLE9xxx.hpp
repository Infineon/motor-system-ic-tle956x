/*!
 * \file        TLE9xxx.hpp
 * \name        TLE9xxx.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9xxx
 * \author      Infineon Technologies AG
 * \copyright   2021 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This library includes the generic functions for TLE9xxx family
 * \ref         Tle9xxxcorelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9XXX_HPP_
#define TLE9XXX_HPP_

#include <stdint.h>
#include <stdlib.h>

#include <Arduino.h>

#include "TLE9xxx-reg.hpp"
#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

/*! \brief SPI address commands */
#define TLE9xxx_CMD_WRITE          	0x80
#define TLE9xxx_CMD_READ          	0x00
#define TLE9xxx_CMD_CLEAR          	0x80

#define DETAILED_ERROR_REPORT 		1				/* print register values as well if a TLE error occurs */

/**
 * @addtogroup tle9xxxapi
 * @{
 */

typedef struct
	{
		uint8_t HBmode;
		uint8_t Freewheeling;
		uint8_t PWMenable;
	} HBconfig_t;

/**
 * @brief represents a basic TLE9xxx
 *
 * This class provides all generic, a TLE9xxx SBC is capable of.
 * This mainly includes the the SPI read/wrtite functions, Cyclic redundancy check (CRC) setup and interrupt-error settings.
 * 
 * This class is not intended to be used directly. It should bequeath its generic functions to a derived class that adds the virtual defined functions related to a specific SBC.
 */
class Tle9xxx
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
		Tle9xxx();

		//! \brief standard destructor switches shield off
		~Tle9xxx();

		/**
		 * @brief virtual function definitions, will be declared in the inheritance sub-classes like Tle9563, Tle9562, ...
		 * 
		 */
		virtual void 			begin();
		virtual void 			end();
		virtual void 			config();
		//virtual void 			setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3);
		//virtual void 			setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3, HBconfig_t hb4);
		//virtual void 			setHSS(uint16_t hss1, uint16_t hss2, uint16_t hss3);

		/**
		 * @brief set the Interrupt Mask to select which events should cause a hardware pin interrupt generation
		 * 
		 */
		void 					configInterruptMask(void);

		/**
		 * @brief Print an Error message, if an interrupt occurs and TLE status register contains an error
		 * 
		 * @param msg hand over error code
		 * @param RegAddress address of the register, the error bit was set in
		 * @param RegContent full content of the register, ther error bit was set in
		 */
		bool					PrintTLEErrorMessage(uint8_t msg, uint16_t &RegAddress, uint16_t &RegContent);
		void 					PrintBinary(uint8_t digits, uint16_t number);
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
		SPIC     				*sBus;      	//<! \brief SPI cover class as representation of the SPI bus
		GPIO     				*csn;        	//<! \brief shield enable GPIO to switch chipselect on/of
		Timer    				*timer;     	//<! \brief timer for delay settings

	protected:
		/**
		 * @brief disable the cyclic redundancy check of TLE9xxx
		 */
		void 					SBC_CRC_Disable();

		/**
		 * @brief send SPI comands to the TLE9xxx
		 * If CRC is disabled, static pattern will be added automatically.
		 * TODO: insert another layer below writeReg, so this function can be used for SPI as well as for CAN and Lin
		 * 
		 * @param addr addres of the register you want access (use the enum in TLE9xxx-reg.cpp)
		 * @param data data you want to transmitt (16-bit)
		 */
		void 					writeReg(uint8_t addr, uint16_t data);

		/**
		 * @brief read SPI commands from TLE9xxx
		 * 
		 * @param addr 		addres of the register you want to read from (use the enum in TLE9xxx-reg.cpp)
		 * @return uint16_t returned value of TLE9xxx (16-bit)
		 */
		uint16_t 				readReg(uint8_t addr);

};
/** @} */

#endif /** TLE9xxx_HPP_ **/
