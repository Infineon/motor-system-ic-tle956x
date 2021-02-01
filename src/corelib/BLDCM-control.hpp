/*!
 * \file        BLDCM-control.hpp
 * \name        BLDCM-control.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020-2021 Infineon Technologies AG
 * \version     1.0.0
 * \brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
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

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)
#include "../TLE9563-ino.hpp"
#endif

// ================================== Defines ==================================================================================================
#define TIMEOUT						500				/* milliseconds. How long no commutation may occur until it can be assumed, the motor got stuck */
#define PI_UPDATE_INTERVAL			100				/* milliseconds. How often should the PI regulator be called. Affects precision if too low. */
#define RPM_DUTYCYCLE_AT_START		80				/* dutycycle, when motor starts to turn before RPM algorithm will be switched on */
#define OPEN_LOOP_DUTYCYCLE			80				/* dutycycle for blind commutation at motor start (open loop) */
#define DUTYCYCLE_TOP_LIMIT			255				/* maximum dutycycle */
#define DUTYCYCLE_BOTTOM_LIMIT		30				/* minimum dutycycle, below the motor won't turn anymore */

#define DUTYCYCLE_SINGLE_STEP       30				/* dutycycle for single stepping in the 'Find Polepairs' function */
#define DETAILED_ERROR_REPORT 		1				/* print register values as well if a TLE error occurs */


/* Braking modes */
#define BRAKEMODE_PASSIVE						0
#define BRAKEMODE_ACTIVE						1
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
			BLDC_PERCENTAGE = 1,	/*Percentage*/
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
		uint8_t					checkBLDCshield();

		/**
		 * @brief hand over the configuration parameters like motor Mode, etc
		 * Needs to be called once in the user code before the motor gets started.
		 * @param MyParameters struct element with all necessary motor parameters.
		 * @return uint8_t Error return code / status report
		 */
		uint8_t 				configBLDCshield();

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
		void					StartBLDCM(void);

		/**
		 * @brief Stop a BLDC motor be either switching off all halfbridges or let the float (TODO)
		 * 
		 * @param brakemode BRAKEMODE_PASSIVE or BRAKEMODE_ACTIVE
		 * @return uint8_t returns success or failure of stopping
		 */
		uint8_t					StopBLDCM(uint8_t brakemode);

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
		 * @brief generate an instance of a TLE9563 controller used on this board
		 * 
		 */
		Tle9563 				*controller;

	// =============================================== BLDCM-frontend.cpp ===============================================================
		/**
		 * @brief Lets the motor single step and prints out the total amount of steps, the commutationstate and hallpattern if demanded
		 * 
		 * @param delay how many milliseconds between the commutations
		 * @param hallsensor if hallsensor is available, set to one, else to zero
		 */
		void 					FindPolepairs(uint16_t delay, bool hallsensor);

		/**
		 * @brief Print a debug message, e.g. if configuration Parameter are missing
		 * 
		 * @param msg hand over the error code
		 */
		void					PrintErrorMessage(_ErrorMessages msg);

		/**
		 * @brief Print an Error message, if an interrupt occurs and TLE status register contains an error
		 * 
		 * @param msg hand over error code
		 * @param RegAddress address of the register, the error bit was set in
		 * @param RegContent full content of the register, ther error bit was set in
		 */
		void					PrintTLEErrorMessage(uint8_t msg, uint16_t &RegAddress, uint16_t &RegContent);





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
		 * @brief only for FindPolepairs frontend function
		 * 
		 * @param dutycycle dutycycle for single step operation (should be low)
		 * @param hallsensor if hallsensor used 1, else 0
		 * @return uint8_t hallpattern if demanded
		 */
		uint8_t 				CommutateHallBLDC(uint8_t dutycycle, bool hallsensor);

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
		uint8_t					_oldPattern = 0;
		uint8_t					_latestPattern = 0;
		uint16_t 				_StepCounter = 0;
		unsigned long			_PI_Update_Timeout = 999999999;
		float 					_PI_Integral = 5000.0; 
		float 					_NumberofSteps = 0;
		uint8_t					_debug_counter = 0;
		//MotorModes				BLDCMotorMode = 0;

	// =============================================== BLDCM-frontend.cpp ===============================================================
		/**
		 * @brief fill up the missing '0's before a binary number
		 * 
		 * @param digits how many digits
		 * @param number The number you want to print (max 16 bit)
		 */
		void 					PrintBinary(uint8_t digits, uint16_t number);


};

/** @} */
#endif /** BLDCMCONTROL_HPP_ **/