/*!
 * @file        BLDCM-control.hpp
 * @name        BLDCM-control.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with TLE9563
 * @author      Infineon Technologies AG
 * @copyright   2022 Infineon Technologies AG
 * @brief       This library includes the basic functions to control a BLDC motor using an instance of TLE9563
 * @ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BLDCMCONTROL_HPP_
#define BLDCMCONTROL_HPP_

#include "TLE9563.hpp"

#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"
#include "../pal/adc.hpp"
#include "../util/BLDCM-logger.hpp"

#include <Arduino.h>

/**
 * Main place to configure BLDC motor parameters. All defines beginning with "CONF_" are intended to be changed by the user. 
 * All other defines should remain as they are.
 */
// ================================== Defines ==================================================================================================

#define CONF_TIMEOUT						500			/*!< milliseconds. How long no commutation may occur until it can be assumed, the motor got stuck */
#define CONF_PI_UPDATE_INTERVAL				100			/*!< milliseconds. How often should the PI regulator be called. Affects precision if too low. */

#define CONF_RPM_DUTYCYCLE_AT_START			80			/*!< dutycycle when motor starts to turn before RPM controller will be switched on */
#define CONF_OPEN_LOOP_DUTYCYCLE			80			/*!< dutycycle for blind commutation at motor start (open loop) */
#define CONF_OPEN_LOOP_DELAY_START			5000		/*!< microseconds. This is the delay between the first commutations when starting a BLDC motor */
#define CONF_OPEN_LOOP_DELAY_LIMIT			1200		/*!< microseconds. The smallest delay that is used before open loop commutation turns into closed loop */
#define CONF_OPEN_LOOP_DELAY_SLOPE			200			/*!< microseconds. The amount CONF_OPEN_LOOP_DELAY_START will be decreased every open loop commutation. You can calculate the amount: O_L_commutations = (5000-1200)/200 */

#define CONF_DUTYCYCLE_TOP_LIMIT			255			/*!< maximum dutycycle */
#define CONF_DUTYCYCLE_BOTTOM_LIMIT			10			/*!< minimum dutycycle, below the motor won't turn anymore */


/****************** Current measurment *******************/
#define SHUNT_RESISTOR_VALUE				0.005     	// Ohm

/****************** Braking modes *******************/
#define BRAKEMODE_PASSIVE						0
#define BRAKEMODE_ACTIVE						1

enum _Halfbridges{
			PHASE1,
			PHASE2,
			PHASE3
		};
// =============================================================================================================================================

/**
 * @brief contains functions to control various types of BLDC motors using a TL9563 SBC
 * 
 * With this class you are able to run a BLDC motor in BEMF mode or hallsensor mode using block commutation.
 * 
 */
class BLDCMcontrol
{

	public:
		/**
		 * @brief enum for BLDC motor control modes
		 */
		enum _MotorModes{
			BLDC_BEMF = 1,		/*Back Electromotive Force*/
			BLDC_HALL = 2,		/*Hall sensor position feedback*/
			BLDC_FOC  = 3		/*Field oriented control*/
		};
		enum _SetSpeedModes{
			BLDC_DUTYCYCLE = 1,	/*Percentage*/
			BLDC_RPM = 2,		/*Rounds per Minute*/
			BLDC_POSITION  = 3	/*Position angle (future)*/
		};
		enum _ErrorMessages{
			PARAMETER_MISSING = 1,
			PARAMETER_OUT_OF_RANGE = 2
		};

		typedef struct{
			_MotorModes feedbackmode;
			_SetSpeedModes speedmode;
			uint8_t MotorPolepairs = 0;
			float PI_Reg_P = 0.01;
			float PI_Reg_I = 0.01;
		}BLDCParameter;

		BLDCParameter 			MotorParam;
		volatile uint8_t 		interrupt_status_changed = 0;
		uint8_t					debug_switch_enable = 0;

		BLDCMcontrol(void);
		~BLDCMcontrol(void);

		/**
		 * @brief serve the BLDC-concerning functions periodically
		 * Needs to be called periodically in the user main routine.
		 * @return uint8_t Error return code / status report
		 */
		uint8_t					serveBLDCshield(void);

		/**
		 * @brief check is status information is available
		 * If information is available, it will be printed automatically by the frontend.
		 * @param request select the registers/information that you want to observe
		 * @return uint8_t uint8_t Error return code / status report
		 */
		uint8_t					checkTLEshield();

		/**
		 * @brief config the TLE9563 chip with customized settings
		 * needs to be called once before the motor will be started
		 * 
		 * @param agc Adaptive Gate Precharge controlloop: _Config_AGC
		 * @return uint8_t 
		 */
		uint8_t 				configBLDCshield(uint8_t agc = AGC_ACTIVE);

		/**
		 * @brief set color and brightness of the onboard RGB-LED
		 * 
		 * Uses the the highside switches and PWM modules of TLE9563.
		 * Each color has a 10-bit accuracy.
		 * @param red 		brightness of the red LED (10-bit)
		 * @param green 	brightness of the green LED (10-bit)
		 * @param blue 		brightness of the blue LED (10-bit)
		 */
		void 					setLED(uint16_t red, uint16_t green, uint16_t blue);

		/**
		 * @brief set speed by handing over the dutycycle to the global _DutyCycle variable
		 * 
		 * @param dutycycle speed of the BLDCM (8-bit)
		 */
		void					setBLDCspeed(uint32_t speed, bool direction, uint8_t fieldweakening = 0);

		/**
		 * @brief do a blind commutation to start up the BLDC in BEMF mode
		 * 
		 */
		void					startBLDCM(void);

		/**
		 * @brief Stop a BLDC motor be either switching off all halfbridges or let the float (TODO)
		 * 
		 * @param brakemode BRAKEMODE_PASSIVE or BRAKEMODE_ACTIVE
		 * @return uint8_t returns success or failure of stopping
		 */
		uint8_t					stopBLDCM(uint8_t brakemode);

		/**
		 * @brief initalize GPIOs and PWM pins
		 * 
		 */
		void 					begin();

		/**
		 * @brief Deinitialize GPIOs and PWM pins
		 * 
		 */
		void 					end();

		/**
		 * @brief only for FindPolepairs frontend function
		 * 
		 * @param dutycycle dutycycle for single step operation (should be low)
		 * @param commuation_step [0;5] which should be the next step
		 * @param hallsensor if hallsensor used 1, else 0
		 * @return uint8_t hallpattern if demanded
		 */
		uint8_t 				commutateHallBLDC(uint8_t dutycycle, uint8_t commutation_step, bool hallsensor);

		/**
		 * @brief Initializes the algorithm for rise/falltime regulation
		 * 
		 * @param hb which halfbridges should be adjusted [PHASE1;PHASE3]
		 */

		void					setupRiseFallTimeRegulation(uint8_t hb);
		
		/**
		 * @brief reads out the actual MOSFET rise-time (fall-time) and compares it to the desired rise-(fall-)time.
		 * The algorithm then adjusts the charge current ICHG for the active MOSFET of the selected halfbridge.
		 * 
		 * @param hb on which halfbridge should the algorithm be applied. Must be the same halfbridge where the PWM is routed to.
		 * @param risetime hands over the actual rise-time
		 * @param falltime hands oder the actual fall-time
		 */
		void					riseFallTimeRegulation(uint8_t hb, uint8_t * iCharge, uint8_t * iDischarge, uint8_t * risetime, uint8_t * falltime);

		/**
		 * @brief Set the T_Rise and T_Fall target times where the regulation loop should go to
		 * 
		 * @param trise_tg rise time target [0;63]
		 * @param tfall_tg fall time target [0;63]
		 */
		void					setTrisefallTarget(uint8_t trise_tg, uint8_t tfall_tg);
		
		/**
		 * @brief Get the Current flowing in the BLDC shield
		 * The maximum current that can be measured with G_DIFF20 is 49,8A.
		 * The resolution is 48,7mA.
		 * @return float returns the current in milliAmps
		 */
		float					getCurrent(void);

		/**
		 * @brief generate an instance of a TLE9563 controller used on this board
		 * 
		 */
		Tle9563 				*controller;
		//Tle9563					*controller = new Tle9563();

	// =============================================== BLDCM-frontend.cpp ===============================================================
		/**
		 * @brief Print a debug message, e.g. if configuration Parameter are missing
		 * 
		 * @param msg hand over the error code
		 */
		void					PrintErrorMessage(_ErrorMessages msg);





	protected:
		AnalogDigitalConverter	*pwmU;			//<! \brief PWM output for phase U
		AnalogDigitalConverter	*pwmV;			//<! \brief PWM output for phase V
		AnalogDigitalConverter	*pwmW;			//<! \brief PWM output for phase W

		GPIO					*bemfU;			//<! \brief BEMF U input from analog comparator
		GPIO					*bemfV;			//<! \brief BEMF V input from analog comparator
		GPIO					*bemfW;			//<! \brief BEMF W input from analog comparator

		GPIO					*hallA;			//<! \brief Hall input A
		GPIO					*hallB;			//<! \brief Hall input B
		GPIO					*hallC;			//<! \brief Hall input C

		Timer    				*timer;     	//<! \brief local timer for delay settings
		Timer					*rpmtimer;		//<! \brief global timer that needs to run while other functions are executed


		/**
		 * @brief check, if the BEMF state changed since the last time and commutate if necessary
		 * (non blocking function)
		 * 
		 * @return uint8_t returns success or failure of startup / commutation
		 */
		uint8_t					DoBEMFCommutation(void);

		/**
		 * @brief check, if the HALL state changed since the last time and commutate if necessary
		 * (non blocking function)
		 * 
		 * @return uint8_t returns success or failure of startup / commutation
		 */
		uint8_t					DoHALLCommutation(void);

		/**
		 * @brief implement a PI regulator that sets the dutycycle in order to keep a desired RPM
		 * 
		 * @param desired_rpmSpeed 
		 */
		void    				PI_Regulator_DoWork(void);

		/**
		 * @brief Read the status of the three hallsensor pins and merge to a binary pattern
		 * Bit 2 is hallA
		 * Bit 1 is hallB
		 * Bit 0 is hallC
		 * @return uint8_t the merged pattern
		 */
		uint8_t 				ReadHallSensor(void);

		/**
		 * @brief Read the status of the three BEMF analog comparator outputs and merge to a binary pattern
		 * Bit 2 is bemfU
		 * Bit 1 is bemfV
		 * Bit 0 is bemfW
		 * @return uint8_t the merged pattern
		 */
		uint8_t					ReadBEMFSensor(void);

		/**
		 * @brief contains the six steps of the six-step-blockcommutation
		 * in each step the gatedrivers of the TLE9563 are updated to either PWM, Ground or Floating.
		 * At the same time the PWM is assigned to the right output pin.
		 * 
		 * @param CommutationStep the actual commutation state, values 0 to 5 are allowed
		 */
		void					UpdateHardware(uint8_t CommutationStep);

		/**
		 * @brief contains a mapping to know what is the next commutation step when a certain hall pattern is available
         * The first index switches between normal mode and fast mode (field weakening range)
         * The second index switches the direction between forward and backward
         * The third index for the pattern is the Hallsensor input as a decimal value (e.g 0b00000101 (BIN) is 5 (DEC))
         * The value at the index position delivers the commutation state for the next step.
         */
        uint8_t HallPattern[2][2][7] = {
            {
                {9, 2,0,1,4,3,5},               /* Normal foreward */
                {9, 5,3,4,1,0,2}                /* Normal backward */
            },
            {
                {9, 3,1,2,5,4,0},               /* Fast foreward */
                {9, 4,2,3,0,5,1}                /* Fast backward */
            }
        };

		/**
		 * @brief contains a mapping to know what is the next commutation step when a certain BEMF pattern is available
         * The first index switches between normal mode and fast mode (field weakening range) (NOT WORKING IN BEMF MODE)
         * The second index switches the direction between forward and backward
         * The third index for the pattern is the BEMF input as a decimal value (e.g 0b00000101 (BIN) is 5 (DEC))
         * The value at the index position delivers the commutation state for the next step.
         */
        uint8_t BEMFPattern[2][2][7] = {
            {
                {9, 2,0,1,4,3,5},               /* Normal foreward */
                {9, 1,5,0,3,2,4}                /* Normal backward */
            },
            {
                {9, 3,1,2,5,4,0},               /* Fast foreward */
                {9, 0,5,3,0,1,1}                /* Fast backward incomp */ 
            }
        };


		uint8_t					_DutyCycle = 0;
		uint16_t				_RefRPM = 0;
		uint8_t					_MotorStartEnable = 0;
		uint8_t					_Direction = 0;
		uint8_t					_FieldWeakening = 0;
		uint8_t					_LastBLDCspeed = 0;
		uint8_t					_Commutation = 0;
		uint8_t					_commutationStep = 0;
		uint8_t					_oldPattern = 0;
		uint8_t					_latestPattern = 0;
		uint16_t 				_StepCounter = 0;
		unsigned long			_PI_Update_Timeout = 999999999;
		float 					_PI_Integral = 5000.0; 
		float 					_NumberofSteps = 0;
		uint8_t					_debug_counter = 0;
		bool					_RFTReg_enable = 0;
		uint8_t					_RFTReg_phase = 0;
		uint8_t *				_RFT_iCharge = NULL;
		uint8_t *				_RFT_iDischarge = NULL;
		uint8_t *				_RFT_risetime = NULL;
		uint8_t *				_RFT_falltime = NULL;

};

/** @} */
#endif /** BLDCMCONTROL_HPP_ **/