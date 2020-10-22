/*!
 * \file        TLE9563.hpp
 * \name        TLE9563.hpp - basic register API
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to controll the TLE9563 registers
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_HPP_
#define TLE9563_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "tle9563-reg.hpp"
#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"

//======= Very important ========
#define DEBUG_OUTPUT         //Uncomment, if you wish debug output or tune the motor (Disables automatic V_neutralOffset)
//===============================

#ifdef DEBUG_OUTPUT
    #define DEBUG_PRINT_LN(str)  Serial.println(str)
    #define DEBUG_PRINT(str)  Serial.print(str)
    #define TRIGGER_PIN  digitalWrite(12, _debugPin); _debugPin = !_debugPin;
#else
	#define DEBUG_PRINT_LN(str)
    #define DEBUG_PRINT(str)
    #define TRIGGER_PIN
#endif

/**
 * @addtogroup tle9563api
 * @{
 */

/**
 * @brief represents a basic TLE9563
 *
 * This class provides a simple API for connecting and controlling motors.
 * Each motor is assigned to a Tle9563 which acts as output driver. Calls to
 * Tle9563Motor instances are mapped to calls to Tle9563. Therefore, this
 * class does not bring new features, it does only provide further abstraction.
 *
 * @see Tle9563
 */
	typedef struct
	{
		uint8_t HBmode;
		uint8_t Freewheeling;
		uint8_t PWMenable;
	} HBconfig_t;

class Tle9563
{
	public:

		//! \brief standard constructor with default pin assignment
		Tle9563();

		//! \brief standard destructor switches shield off
		~Tle9563();

		void config(void);
		void setHalfbridge(HBconfig_t hb1, HBconfig_t hb2, HBconfig_t hb3);

		virtual void begin() = 0;
		virtual void end() = 0;

		HBconfig_t ActiveGround; 
		HBconfig_t ActivePWM; 
		HBconfig_t Floating; 

		GPIO     				*intn;        	//<! \brief interrupt / test input
		AnalogDigitalConverter	*cso;			//<! \brief Current sense amplifier input

		AnalogDigitalConverter	*pwmU;			//<! \brief PWM output for phase U
		AnalogDigitalConverter	*pwmV;			//<! \brief PWM output for phase V
		AnalogDigitalConverter	*pwmW;			//<! \brief PWM output for phase W

		GPIO					*bemfU;			//<! \brief BEMF U input from analog comparator
		GPIO					*bemfV;			//<! \brief BEMF V input from analog comparator
		GPIO					*bemfW;			//<! \brief BEMF W input from analog comparator

		GPIO					*hallA;			//<! \brief Hall input A
		GPIO					*hallB;			//<! \brief Hall input B
		GPIO					*hallC;			//<! \brief Hall input C
	
		Timer    				*timer;     	//<! \brief timer for delay settings

	protected:

		virtual void SBC_CRC_Disable() = 0;						// Declared in TLE9563-pal-ino.cpp
		virtual void SBC_SPI(uint8_t addr, uint16_t data) = 0;	// Declared in TLE9563-pal-ino.cpp

		SPIC     				*sBus;      	//<! \brief SPI cover class as representation of the SPI bus
		GPIO     				*csn;        	//<! \brief shield enable GPIO to switch chipselect on/of

};
/** @} */

#endif /** TLE9563_HPP_ **/
