/*!
 * \file        BLDCM-control.hpp
 * \name        BLDCM-control.hpp - Arduino library to control Infineon's BLDC Motor Control Shield with Tle9563
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     0.0.1
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

#define TIMEOUT		100		/* milliseconds. How long no commutation may occur until it can be assumed, the motor got stuck */

/* Braking modes */
#define PASSIVE		0
#define ACTIVE		1

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
		enum MotorModes{
			TLE_BEMF = 1,		/*Back Electromotive Force*/
			TLE_HALL = 2,		/*Hall sensor position feedback*/
			TLE_FOC  = 3		/*Field oriented control*/
		};

		typedef struct{
			MotorModes mode;

		}BLDCParameter;

		BLDCParameter MotorParam;

		BLDCMcontrol(void);
		~BLDCMcontrol(void);

		/**
		 * @brief serve the BLDC-concerning functions periodically
		 * Needs to be called periodically in the user main routine.
		 * @return uint8_t Error return code / status report
		 */
		uint8_t					serveBLDCshield(void);

		/**
		 * @brief hand over the configuration parameters like motor Mode, etc
		 * Needs to be called once in the user code before the motor gets started.
		 * @param MyParameters struct element with all necessary motor parameters.
		 * @return uint8_t Error return code / status report
		 */
		uint8_t 				configBLDCshield(BLDCParameter MyParameters);

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
		void					setBLDCspeed(uint8_t dutycycle, bool direction, uint8_t fieldweakening = 0);

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

		Timer    				*timer;     	//<! \brief timer for delay settings

		/**
		 * @brief do a blind commutation to start up the BLDC in BEMF mode
		 * 
		 */
		void					StartBLDCM(void);

		/**
		 * @brief Stop a BLDC motor be either switching off all halfbridges or let the float (TODO)
		 * 
		 * @param brakemode PASSIVE or ACTIVE
		 * @return uint8_t returns success or failure of stopping
		 */
		uint8_t					StopBLDCM(uint8_t brakemode);

		/**
		 * @brief depending on the actual commutation state, wait for the next BEMF flag to commutate
		 * 
		 * @return uint8_t returns success or failure of startup / commutation
		 */
		uint8_t					DoBEMFCommutation(void);

		/**
		 * @brief depending on the actual commutation state, wait for the next BEMF flag to commutate
		 * 
		 * @return uint8_t returns success or failure of startup / commutation
		 */
		uint8_t					DoHALLCommutation(void);

		/**
		 * @brief wait until the hallpattern changes
		 * This function is executed until either the hall sensor pattern changes or a timer is out of time.
		 * The timer is a kind of watchdog that makes sure, that the program gets not stuck, if the dutycycle was too low.
		 * @return true commutation successfull
		 * @return false watchdog timeout
		 */
		bool 					WaitForCommutation(void);

		/**
		 * @brief Read the status of the three hallsensor pins and merge to a binary pattern
		 * Bit 2 is hallA
		 * Bit 1 is hallB
		 * Bit 0 is hallC
		 * @return uint8_t the merged pattern
		 */
		uint8_t 				ReadHallSensor(void);

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

		uint8_t					_DutyCycle = 0;
		uint8_t					_Direction = 0;
		uint8_t					_FieldWeakening = 0;
		uint8_t					_LastBLDCspeed = 0;
		uint8_t					_Commutation = 0;
		uint8_t					_oldHall = 0;
		uint8_t					_latestHall = 0;
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