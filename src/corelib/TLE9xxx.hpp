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
/**
 * @brief Main place to configure the TLE9xxx. All defines beginning with "CONF_" are intended to be changed by the user. 
 * All other defines should remain as they are.
 */

#define DETAILED_ERROR_REPORT 		1						// 1 = print register values as well if a TLE error occurs, 0 = only print error message

/****************** Adaptive Gate control (dis-)charge current *******************/
#define CONF_TRISE_TG				11						// [0;63] initial Target tRISE (CONF_TRISE_TG * 53.3 ns). The variable can be changed afterwards.
#define CONF_INIT_ICHG              11						// [0;63] Starting charge current that will be first used by the algorithm
#define CONF_TFALL_TG				11						// [0;63] initial Target tFALL (CONF_TFALL_TG * 53.3 ns). The variable can be changed afterwards.
#define CONF_INIT_IDCHG				11						// [0;63] Starting discharge current that will be first used by the algorithm
#define CONF_ICHG_FW				43						// [0;63] Freewheeling charge and discharge current

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
#define EOS                         0xFFFF                  // End of scale of a uint16_t variable (m_trise_ema variable), for initialization purposes

/****************** Adaptive Gate control PRE-(dis-)charge current *******************/
#define CONF_PDCHG_INIT				44		// [0;63] Initial predischarge current, default 0xF
#define CONF_PCHG_INIT				23		// [0;63] Initial precharge current, default 0xD
#define CONF_TDOFF					18		// [0;63] Turn-on delay time, default 0xC
#define CONF_TDON					20		// [0;63] Turn-off delay time, default 0xC
#define CONF_TPCHGX					1		// [0;7]  Precharge time, default 0
#define CONF_TPDCHGX				3		// [0;7]  Predischarge time, default 0

/****************** Static (dis-)charge current *******************/
#define CONF_ICHGST					8		// [0;15] Static charge/discharge current, default 4

/****************** General Bridge Control *******************/
#define CONF_BDFREQ					1		// [0;1] Bridge driver synchronization frequency: 37Mhz
											// PWM setting defined in setGenControl(bool MapPWM1, bool MapPWM2)
#define CONF_CPUVTH					0		// [0;1] Charge pump under voltage:	TH1
#define CONF_FET_LVL				1		// [0;1] External MOSFET logic level:	normal level MOSFET
#define CONF_CPSTGA					1		// [0;1] Automatic switchover between dual and single charge pump stage: Normal
#define CONF_BDOV_REC				1		// [0;1] Bridge driver recover from VSINT Overvoltage: ACTIVE
#define CONF_IPCHGADT				0		// [0;1] 1Step
											// Adaptive gate control: configured in a global variable
#define CONF_CPEN					1		// [0;1] charge pump: enabled
#define CONF_POCHGDIS				0		// [0;1] Postcharge phase during PWM: disabled
#define CONF_AGCFILT				0		// [0;1] Filter for adaptive gate control:	NO_FILT
#define CONF_EN_GEN_CHECK			0		// [0;1] detection of active / FW MOSFET: disabled
#define CONF_IHOLD					0		// [0;1] Gate driver hold current:	TH1
#define CONF_FMODE					0		// [0;1] Frequency modulation of charge pump: no modulation

#define PWM3_TO_HB3					0
#define PWM3_TO_HB4					1
#define PWM1_TO_HB1					0
#define PWM1_TO_HB2					1

/****************** HS and LS Drain-Source monitoring threshold *******************/
#define CONF_LS_AND_HS_X_VDSTH		7		// [0;7] Set overvoltage threshold
#define CONF_DEEP_ADAP				1		// [0;1] deep adaption
#define CONF_TFVDS					0		// [0;3] set filter time 0 = 500ns (default)

/****************** Maximum Pre-(Dis)charge in PWM operation *******************/
#define CONF_HBXIDIAG				0		// [0;1] disable pulldown for off-state diagnostic
#define CONF_ICHGMAXX				3		// [0;3] set maximum pre (dis)charge current. 0 = 19mA (default), 1 = 32mA, 2 = 73mA, 3 = 100mA

/****************** Cross current protection *******************/
#define CONF_TBLANK_ACT				11		// [0;15] Set the blank time for active MOSFETS. default is 7
#define CONF_TCCP_ACT				10		// [0;15] Set the current protection time for active MOSFETS. default is 7
#define CONF_TBLANK_FW				1		// [0;15] Set the blank time for free wheeling MOSFETS. default is 7
#define CONF_TCCP_FW				1		// [0;15] Set the current protection time for free wheeling MOSFETS. default is 7

/****************** SPI address commands *******************/
#define TLE9xxx_CMD_WRITE          	0x80
#define TLE9xxx_CMD_READ          	0x00
#define TLE9xxx_CMD_CLEAR          	0x80

/****************** PWM HSS modules *******************/
#define PWM_CTRL_DC_MASK			0x3FF0
#define PWM_BNK_MODULE_1			0x0
#define PWM_BNK_MODULE_2			0x1
#define PWM_BNK_MODULE_3			0x2
#define PWM_BNK_MODULE_4			0x3

#define ACTIVE_MOSFET				0
#define FW_MOSFET					1

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

enum _Config_AGC{
		AGC_INACTIVE1 = 0b00,
		AGC_INACTIVE2 = 0b01,
		AGC_ACTIVE = 0b10
		};

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
			TLE_OC_CSA_OVERCURRENT	= 0x200,
			TLE_HS_LS_OVERVOLTAGE 	= 0x100,
			TLE_SPI_ERROR 			= 0x80,
			TLE_LOAD_ERROR 			= 0x40,
			TLE_UNDER_VOLTAGE 		= 0x20,
			TLE_OVER_VOLTAGE 		= 0x10,
			TLE_POWER_ON_RESET 		= 0x08,
			TLE_TEMP_SHUTDOWN 		= 0x04,
			TLE_OVERCURRENT 		= 0x02,
			TLE_SHORT_CIRCUIT 		= 0x01
		};
		enum SIF
		{
			SIF_SPI_CRC_FAIL = 	0x80,
			SIF_BD_STAT = 		0x40,
			SIF_DEV_STAT = 		0x20,
			SIF_HS_STAT = 		0x10,
			SIF_WAKE_UP = 		0x08,
			SIF_BUS_STAT = 		0x04,
			SIF_TEMP_STAT = 	0x02,
			SIF_SUPPLY_STAT = 	0x01
		};
		enum _Halfbridges{
			PHASE1,
			PHASE2,
			PHASE3,
			PHASE4
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
		 * @brief this function will be called if an an interrupt occured. It will first gather information about the error out of the
		 * Status information field. Depending on which bits are set there, the specific register values will be read for more information.
		 * 
		 * @return uint8_t status information field content
		 */
		uint16_t				checkStatusInformationField(void);

		/**
		 * @brief Print an Error message, if an interrupt occurs and TLE status register contains an error
		 * 
		 * @param msg hand over error code
		 * @param RegAddress address of the register, the error bit was set in
		 * @param RegContent full content of the register, ther error bit was set in
		 */
		bool					PrintTLEErrorMessage(uint16_t msg, uint16_t &RegAddress, uint16_t &RegContent);
		void 					PrintBinary(uint8_t digits, uint16_t number);
		/**
		 * @brief read the Supply Voltage Fail Status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint16_t				checkStatSUP(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the Thermal Protection status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint16_t				checkStatTHERM(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the High-Side Switch status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint16_t				checkStatHSS(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the Device Information status register of the TLE and generate an ErrorCode depending of the fault-category
		 * 
		 * @param address returns the composed register address and content for detailed debugging
		 * @return DiagFlag return the generated Error code
		 */
		uint16_t				checkStatDEV(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief read the Drain-source overvoltage status register and generate an ErrorCode depending of the fault-category
		 * 
		 * @param RegAddress Adress of DSOV register
		 * @param RegContent Content of DSOV register
		 * @return uint8_t DiagFlag return the generated Error code
		 */
		uint16_t 				checkStatDSOV(uint16_t &RegAddress, uint16_t &RegContent);

		/**
		 * @brief initialize variables for the adaptive gate control algorithm
		 * 
		 */
		void 					init_AGC_Algorithm(uint8_t hb);

		/**
		 * @brief read out rise-time of TLE and store calculated value in a global variable
		 * 
		 */
		void 					emaCalculation(uint8_t hb, uint8_t * risetime, uint8_t * falltime);

		/**
		 * @brief increase or decrease the charge current, depending on some conditions
		 * 
		 */
		void 					adaptiveHysteresisDecisionTree(uint8_t hb, uint8_t * ichg, uint8_t * idchg);

		uint8_t					m_trise_tg = CONF_TRISE_TG;
		uint8_t					m_tfall_tg = CONF_TFALL_TG;

		GPIO     				*intn;        	//<! \brief interrupt / test input
		SPIC     				*sBus;      	//<! \brief SPI cover class as representation of the SPI bus
		GPIO     				*csn;        	//<! \brief shield enable GPIO to switch chipselect on/of
		Timer    				*timer;     	//<! \brief timer for delay settings




	protected:

		/**
		 * @brief set the precharge and pre-discharge time for all MOSFETs.
		 * This function sends two SPI commands.
		 * @param t_pchgx Precharge time
		 * @param t_pdchgx Pre-discharge time
		 */
		void 					set_TPRECHG(uint8_t t_pchgx, uint8_t t_pdchgx);

		/**
		 * @brief set the charge discharge current for static activation (no PWM).
		 * 
		 * @param ichgst1 static charge and discharge currents on PHASE1
		 * @param ichgst2 static charge and discharge currents on PHASE2
		 * @param ichgst3 static charge and discharge currents on PHASE3
		 * @param ichgst4 static charge and discharge currents on PHASE4
		 */
		void					set_ST_ICHG(uint8_t ichgst1, uint8_t ichgst2, uint8_t ichgst3, uint8_t ichgst4);

		/**
		 * @brief HB charge/discharge currents for PWM operation
		 * REF p.221 in TLE9562 datasheet
		 * @param idchg Discharge current of HBx active MOSFET
		 * @param ichg Charge current of HBx active MOSFET or charge and discharge current of HBx FW MOSFET
		 * @param ACTorFW 0 = active MOSFET, 1 = FW MOSFET
		 * @param hb [0;3] which halfbridge should be changed
		 */
		void					set_HB_ICHG(uint8_t idchg, uint8_t ichg, bool ACTorFW, uint8_t hb);

		/**
		 * @brief HM max. pre-charge/pre-discharge in PWM operation current and diagnostic pull-down
		 * 
		 * @param hbxidiag [0;1] Control of HBx off-state current source and current sink
		 * @param ichgMaxx [0;3] Maximum drive current of all HBs during the pre-charge phase and pre-discharge phases
		 */
		void					set_HB_ICHG_MAX(uint8_t hbxidiag, uint8_t ichgMaxx);

		/**
		 * @brief HBx per-charge/pre-discharge initialization configuration in PWM operation
		 * 
		 * @param pdchg_init Initial predischarge current of HBx
		 * @param pchg_init  Initial precharge current of HBx
		 * @param init_bnk Banking bits for Precharge and Predischarge Initial Current
		 */
		void					set_PCHG_INIT(uint8_t pdchg_init, uint8_t pchg_init, uint8_t init_bnk);

		/**
		 * @brief HBx inouts TDON configuration
		 * 
		 * @param t_don Turn-on delay time of active MOSFET of HBx
		 * @param hb_tdon_bnk Banking bits for turn-on delay time
		 */
		void					set_TDON_HB_CTRL(uint8_t t_don, uint8_t hb_tdon_bnk);

		/**
		 * @brief HBx TDOFF configuration
		 * 
		 * @param t_doff Tirn-off delay time of active MOSFET of HBx
		 * @param hb_tdoff_bnk Banking bits for turn-off delay time
		 */
		void					set_TDOFF_HB_CTRL(uint8_t t_doff, uint8_t hb_tdoff_bnk);

		/**
		 * @brief Set the LS and HS VDS monitoring threshold
		 * 
		 * @param vdsth [0;7]drain-source overvoltage threshold for all HS and LS
		 * @param deep_adap [0;1] Deep Adaption
		 * @param tfvds [0;3] Filter time of drain-source voltage moitoring
		 */
		void					set_LS_and_HS_VDS(uint8_t vdsth, bool deep_adap, uint8_t tfvds);

		/**
		 * @brief Set the current protection times
		 * 
		 * @param t_blank set blank time
		 * @param t_ccp set cross current protection time
		 * @param ACTorFW 0 = active MOSFET, 1 = FW MOSFET
		 * @param hb [0;3] which halfbridge should be changed
		 */
		void					set_CCP_BLK(uint8_t t_blank, uint8_t t_ccp, bool ACTorFW, uint8_t hb);

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
		
		uint16_t        		m_trise_ema[MAX_ICHG - MIN_ICHG + 1];     	// Array of the calculated tRISEx EMAs for the different configured ICHGx
		uint16_t        		m_tfall_ema[MAX_ICHG - MIN_ICHG + 1];     	// Array of the calculated tFALLx EMAs for the different configured IDCHGx
		uint8_t         		m_ichg = CONF_INIT_ICHG;                  	// Current ICHGx with which the MOSFET driver is configured
		uint8_t         		m_idchg = CONF_INIT_IDCHG;                 	// Current IDCHGx with which the MOSFET driver is configured
		uint8_t					_statusInformationField = 0;				// Stores the "Status information field" which is the first byte of every SDO package.
		uint8_t					_agc_status = 0;							// Stores the value to be written

};
/** @} */

#endif /** TLE9xxx_HPP_ **/
