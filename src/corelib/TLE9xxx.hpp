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
// ================================== Defines ==================================================================================================
/****************** SPI address commands *******************/
#define TLE9xxx_CMD_WRITE          	0x80
#define TLE9xxx_CMD_READ          	0x00
#define TLE9xxx_CMD_CLEAR          	0x80

#define DETAILED_ERROR_REPORT 		1						// print register values as well if a TLE error occurs

/****************** Adaptive Gate control *******************/
#define tRISE_TG                   11                 		// Target tRISE (tRISE_TG * 53.3 ns)

/**
 * A value > 0.5 gives more importance to the last tRISE sampled
 * A value < 0.5 gives more importance to the history of sampled tRISE
 */
#define ALPHA3                      0.5                     // ALPHA3, constant for the EMA calculation, 0 <= ALPHA3 <= 1
#define SCALING_FACTOR_FPA          (0xFF)                  // Scaling factor for fixed-point arithmetic (FPA)
#define ALPHA3_FPA_SCALED           (ALPHA3 * SCALING_FACTOR_FPA)               // ALPHA3, already scaled for FPA
#define ONE_MINUS_ALPHA3_FPA_SCALED (SCALING_FACTOR_FPA - ALPHA3_FPA_SCALED)    // (1 - ALPHA3), already scaled for FPA
#define MAX_ICHG                   	63                      // Maximum charge current that will be set by the algorithm
#define MIN_ICHG                   	0                       // Minimum charge current that will be set by the algorithm
#define INIT_ICHG                  	11                      // Starting charge current that will be first used by the algorithm
#define EOS                         0xFFFF                  // End of scale of a uint16_t variable (m_trise_ema variable), for initialization purposes

/****************** General Bridge Control *******************/
#define BDFREQ						1		// Bridge driver synchronization frequency: 37Mhz
											// PWM setting defined in setGenControl(bool MapPWM1, bool MapPWM2)
#define CPUVTH						0		// Charge pump under voltage:	TH1
#define FET_LVL						1		// External MOSFET logic level:	normal level MOSFET
#define CPSTGA						1		// Automatic switchover between dual and single charge pump stage: Normal
#define BDOV_REC					1		// Bridge driver recover from VSINT Overvoltage: ACTIVE
#define IPCHGADT					0		// 1Step
											// Adaptive gate control: configured in a global variable
#define CPEN						1		// charge pump: enabled
#define POCHGDIS					0		// Postcharge phase during PWM: disabled
#define AGCFILT						0		// Filter for adaptive gate control:	NO_FILT
#define EN_GEN_CHECK				0		// detection of active / FW MOSFET: disabled
#define IHOLD						0		// Gate driver hold current:	TH1
#define FMODE						0		// Frequency modulation of charge pump: no modulation

#define PWM3_TO_HB3					0
#define PWM3_TO_HB4					1
#define PWM1_TO_HB1					0
#define PWM1_TO_HB2					1

/****************** PWM HSS modules *******************/
#define PWM_CTRL_DC_MASK			0x3FF0
#define PWM_BNK_MODULE_1			0x0
#define PWM_BNK_MODULE_2			0x1
#define PWM_BNK_MODULE_3			0x2
#define PWM_BNK_MODULE_4			0x3

/****************** PWM HSS modules *******************/

// ===============================================================================================================================================
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

		HBconfig_t 				ActiveGround; 
		HBconfig_t 				ActivePWM; 
		HBconfig_t 				Floating; 

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
		//virtual void 			config();
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

		/**
		 * @brief read the Drain-source overvoltage status register and generate an ErrorCode depending of the fault-category
		 * 
		 * @param RegAddress Adress of DSOV register
		 * @param RegContent Content of DSOV register
		 * @return uint8_t DiagFlag return the generated Error code
		 */
		uint8_t 				checkStatDSOV(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief initialize variables for the adaptive gate control algorithm
		 * 
		 */
		void 					init_AGC_Algorithm(uint8_t hb);

		/**
		 * @brief read out rise-time of TLE and store calculated value in a global variable
		 * 
		 */
		void 					emaCalculation(uint8_t hb, uint8_t &risetime, uint8_t &falltime);

		/**
		 * @brief increase or decrease the charge current, depending on some conditions
		 * 
		 */
		void 					adaptiveHysteresisDecisionTree(uint8_t hb);

		GPIO     				*intn;        	//<! \brief interrupt / test input
		SPIC     				*sBus;      	//<! \brief SPI cover class as representation of the SPI bus
		GPIO     				*csn;        	//<! \brief shield enable GPIO to switch chipselect on/of
		Timer    				*timer;     	//<! \brief timer for delay settings




	protected:
		/**
		 * @brief set the charge discharge current for static activation (no PWM).
		 * 
		 * @param ICHGST1 static charge and discharge currents oh HB1
		 * @param ICHGST2 static charge and discharge currents oh HB2
		 * @param ICHGST3 static charge and discharge currents oh HB3
		 * @param ICHGST4 static charge and discharge currents oh HB4
		 */
		void					set_ST_ICHG(uint8_t ICHGST1, uint8_t ICHGST2, uint8_t ICHGST3, uint8_t ICHGST4);

		/**
		 * @brief HB charge/discharge currents for PWM operation
		 * REF p.221 in TLE9562 datasheet
		 * @param IDCHG Charge current of HBx active MOSFET
		 * @param ICHG Charge current of HBx active MOSFET or charge and discharge current of HBx FW MOSFET
		 * @param ICHG_BNK Banking bits for charge and discharge currents 
		 */
		void					set_HB_ICHG(uint8_t IDCHG, uint8_t ICHG, bool ACTorFW, uint8_t hb);

		/**
		 * @brief HM max. pre-charge/pre-discharge in PWM operation current and diagnostic pull-down
		 * 
		 * @param HBxIDIAG [0;1] Control of HBx off-state current source and current sink
		 * @param ICHGMAXx [0;3] Maximum drive current of all HBs during the pre-charge phase and pre-discharge phases
		 */
		void					set_HB_ICHG_MAX(uint8_t HBxIDIAG, uint8_t ICHGMAXx);

		/**
		 * @brief HBx per-charge/pre-discharge initialization configuration in PWM operation
		 * 
		 * @param PDCHGINIT Initial predischarge current of HBx
		 * @param PCHGINIT  Initial precharge current of HBx
		 * @param INIT_BNK Banking bits for Precharge and Predischarge Initial Current
		 */
		void					set_PCHG_INIT(uint8_t PDCHGINIT, uint8_t PCHGINIT, uint8_t INIT_BNK);

		/**
		 * @brief HBx inouts TDON configuration
		 * 
		 * @param TDON Turn-on delay time of active MOSFET of HBx
		 * @param HB_TDON_BNK Banking bits for turn-on delay time
		 */
		void					set_TDON_HB_CTRL(uint8_t TDON, uint8_t HB_TDON_BNK);

		/**
		 * @brief HBx TDOFF configuration
		 * 
		 * @param TDOFF Tirn-off delay time of active MOSFET of HBx
		 * @param HB_TDOFF_BNK Banking bits for turn-off delay time
		 */
		void					set_TDOFF_HB_CTRL(uint8_t TDOFF, uint8_t HB_TDOFF_BNK);

		/**
		 * @brief Reads out the MOSFET rise/fall time of the given PWM half-bridge
		 * 
		 * @param hb which halfbridge should be read [1-4]
		 */
		void					checkStat_TRISE_FALL(uint8_t hb, uint8_t &Trise, uint8_t &Tfall);

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
		uint8_t 				writeReg(uint8_t addr, uint16_t data);

		/**
		 * @brief read SPI commands from TLE9xxx
		 * 
		 * @param addr 		addres of the register you want to read from (use the enum in TLE9xxx-reg.cpp)
		 * @return uint16_t returned value of TLE9xxx (16-bit)
		 */
		uint16_t 				readReg(uint8_t addr);

		/**
		 * @brief evaluate content if the status  information field and read out corresponding registers if necessary
		 * 
		 * @param sif status information field content (first byte of SPI SDO)
		 */
		void					checkStatusInformationField(uint8_t sif);
		
		uint16_t        		m_trise_ema[MAX_ICHG - MIN_ICHG + 1];     // Array of the calculated tRISEx EMAs for the different configured ICHGx
		uint8_t         		m_ichg = INIT_ICHG;                       // Current ICHGx with which the MOSFET driver is configured
		uint8_t         		m_idchg = INIT_ICHG;                      // Current IDCHGx with which the MOSFET driver is configured

};
/** @} */

#endif /** TLE9xxx_HPP_ **/
