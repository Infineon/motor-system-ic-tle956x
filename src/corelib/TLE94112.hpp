/*!
 * \file        TLE9563.hpp
 * \name        TLE9563.hpp - basic register API
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     2.0.0
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
#include "tle9563_util.hpp"
#include "../pal/timer.hpp"
#include "../pal/gpio.hpp"
#include "../pal/spic.hpp"

/**
 * @addtogroup tle9563api
 * @{
 */

/*! \brief the number of halfbridges on a TLE9563 (including no halfbridge)
 *
 * \see mHalfBridges
 * \see HalfBridge
 */
#define TLE9563_NUM_HB             13

/*! \brief the number of pwm modes for a halfbridge (including no pwm)
 *
 * \see mPwmChannels
 * \see PWMChannel
 */
#define TLE9563_NUM_PWM            4

/*! \brief the number of control registers in a TLE9563
 *
 * \see CtrlRegisters
 * \see mCtrlRegAddresses
 * \see mCtrlRegData
 */
#define TLE9563_NUM_CTRL_REGS      12

/*! \brief the number of status registers in a TLE9563
 *
 * \see StatusRegisters
 * \see mStatusRegAddresses
 * \see mStatusRegData
 */
#define TLE9563_NUM_STATUS_REGS    7

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
class Tle9563
{
	public:
		//! \brief enum for the halfbridges on a TLE9563
		enum HalfBridge
		{
			TLE_NOHB = 0, TLE_HB1, TLE_HB2, TLE_HB3, TLE_HB4, TLE_HB5, TLE_HB6,
			TLE_HB7, TLE_HB8, TLE_HB9, TLE_HB10, TLE_HB11, TLE_HB12
		};

		//! \brief enum for the PWM channels of a halfbridge on TLE9563
		enum PWMChannel
		{
			TLE_NOPWM = 0, TLE_PWM1, TLE_PWM2, TLE_PWM3
		};

		//! \brief enum for the output states of a halfbridge
		enum HBState
		{
			TLE_FLOATING = 0b00, TLE_LOW = 0b01, TLE_HIGH = 0b10
		};

		//! \brief enum for the frequencies of a PWM channel
		enum PWMFreq
		{
			TLE_FREQOFF = 0b00, TLE_FREQ80HZ, TLE_FREQ100HZ, TLE_FREQ200HZ
		};

		//! \brief enum for the flags in the register SYS_DIAG1
		enum DiagFlag
		{
			TLE_SPI_ERROR = 0x80,
			TLE_LOAD_ERROR = 0x40,
			TLE_UNDER_VOLTAGE = 0x20,
			TLE_OVER_VOLTAGE = 0x10,
			TLE_POWER_ON_RESET = 0x08,
			TLE_TEMP_SHUTDOWN = 0x04,
			TLE_TEMP_WARNING = 0x02
		};
		//! \brief Reference value of Status Register
		static const uint8_t TLE_STATUS_OK = 0U;

		//! \brief standard constructor with default pin assignment
		Tle9563();

		/*! \brief constructor with individual pin assignment
		 *
		 * \param bus  a pointer to the object representing the SPI class
		 * \param csPin  pin number of the CS pin
		 */
		Tle9563(void* bus, uint8_t csPin);

		//! \brief standard destructor switches shield off
		~Tle9563();

		//! \brief enables and initializes the TLE9563
		void begin(void);

		//! \brief deactivates all outputs and disables the TLE9563
		void end(void);

		/*! \brief sets the output state and the PWM channel for a halfbridge (only for passive freewheeling)
		 *
		 * \param hb    halfbridge which will be configured
		 * \param state sets the output voltage to high, low or floating
		 * \param pwm   selects a PWM channel for PWM output
		 *
		 * \see HalfBridge
		 * \see HBState
		 * \see PWMChannel
		 */
		void configHB(HalfBridge hb, HBState state, PWMChannel pwm);

		/*! \brief sets the output state and the PWM channel for a halfbridge (allows active freewheeling)
		 *
		 * \param hb       halfbridge which will be configured
		 * \param state    sets the output voltage to high, low or floating
		 * \param pwm      selects a PWM channel for PWM output
		 * \param activeFW 1 for active freewheeling
		 *                 0 for passive freewheeling (default)
		 *
		 * \see HalfBridge
		 * \see HBState
		 * \see PWMChannel
		 */
		void configHB(HalfBridge hb, HBState state, PWMChannel pwm, uint8_t activeFW);

		/*! \brief sets the frequency and duty cycle for a PWM channel
		 *
		 * \param pwm       PWM channel which will be configured
		 * \param freq      selects the PWM output frequency
		 * \param dutyCycle a value from 0 to 255 which sets the dutyCycle
		 *
		 * \see PWMChannel
		 * \see PWMFreq
		 */
		void configPWM(PWMChannel pwm, PWMFreq freq, uint8_t dutyCycle);

		/*! \brief returns a diagnosis value for error detection
		 *
		 * \return 0 if everything is ok
		 *         non-zero value if errors occurred
		 */
		uint8_t getSysDiagnosis();

		/*! \brief shows if errors of a specific type have occurred
		 *
		 * \param mask mask to filter for a specific flag
		 *
		 * \return 0 if everything is ok
		 *         non-zero value on error condition
		 *
		 * \see DiagFlag
		 */
		uint8_t getSysDiagnosis(DiagFlag mask);

		/*! \brief shows if errors of a specific type have occurred
		 *
		 * \param mask mask to filter for one or more specific flags
		 *             this can be a disjunction of DiagFlag values
		 *
		 * \return 0 if everything is ok
		 *         non-zero value on error condition
		 *
		 * \see DiagFlag
		 */
		uint8_t getSysDiagnosis(uint8_t mask);

		/*! \brief gets the overcurrent error flag bit for a specific halfbridge
		 *
		 * \param hb halfbridge thats overcurrent flag will be returned
		 *
		 * \return 1 if there HalfBridge was shut down because of an overcurrent
		 *         0 otherwise
		 */
		uint8_t getHBOverCurrent(HalfBridge hb);

		/*! \brief gets the openload error flag bit for a specific halfbridge
		 *
		 * \param hb halfbridge thats openload flag will be returned
		 *
		 * \return 1 if there HalfBridge detected an open load
		 *         0 otherwise
		 */
		uint8_t getHBOpenLoad(HalfBridge hb);

		//! \brief clears all clearable error flags
		void clearErrors();

		SPIC     *sBus;      //<! \brief SPI cover class as representation of the SPI bus
		GPIO     *en;        //<! \brief shield enable GPIO to switch shield on/of
		GPIO     *cs;        //<! \brief shield enable GPIO to switch chipselect on/of
		Timer    *timer;     //<! \brief timer for delay settings

	protected:

		//! \brief enum for the control registers in a TLE9563
		enum CtrlRegisters
		{
			HB_ACT_1_CTRL = 0,
			HB_ACT_2_CTRL,
			HB_ACT_3_CTRL,
			HB_MODE_1_CTRL,
			HB_MODE_2_CTRL,
			HB_MODE_3_CTRL,
			PWM_CH_FREQ_CTRL,
			PWM1_DC_CTRL,
			PWM2_DC_CTRL,
			PWM3_DC_CTRL,
			FW_OL_CTRL,
			FW_CTRL
		};

		//! \brief enum for the control registers in a TLE9563
		enum StatusRegisters
		{
			SYS_DIAG1 = 0,
			OP_ERROR_1_STAT,
			OP_ERROR_2_STAT,
			OP_ERROR_3_STAT,
			OP_ERROR_4_STAT,
			OP_ERROR_5_STAT,
			OP_ERROR_6_STAT
		};

		//! \brief array of register locations for halfbridges
		HalfBridge_t mHalfBridges[TLE9563_NUM_HB];
		//! \brief array of register locations for PWM channels
		PWMchannel_t mPwmChannels[TLE9563_NUM_PWM];
		//! \brief mapping array for control register addresses
		uint8_t mCtrlRegAddresses[TLE9563_NUM_CTRL_REGS];
		//! \brief mirror array for control register data
		uint8_t mCtrlRegData[TLE9563_NUM_CTRL_REGS];
		//! \brief mapping array for status register addresses
		uint8_t mStatusRegAddresses[TLE9563_NUM_STATUS_REGS];

		//! \brief initializes this object, automatically called by begin()
		void init(void);

		/*! \brief sets the output state and the PWM channel for a halfbridge
		 *         automatically called by the public version of configHB
		 *
		 * \param hb       halfbridge which will be configured
		 * \param state    sets the output voltage to high, low or floating
		 * \param pwm      selects a PWM channel for PWM output
		 * \param activeFW 1 for active freewheeling
		 *                 0 for passive freewheeling (default)
		 */
		void configHB(uint8_t hb, uint8_t state, uint8_t pwm, uint8_t activeFW);

		/*! \brief sets the frequency and duty cycle for a PWM channel
		 *         automatically called by the public version of configPWM
		 *
		 * \param pwm       PWM channel which will be configured
		 * \param freq      selects the PWM output frequency
		 * \param dutyCycle a value from 0 to 255 which sets the dutyCycle
		 */
		void configPWM(uint8_t pwm, uint8_t freq, uint8_t dutyCycle);

		/*! \brief gets the overcurrent error flag bit for a specific halfbridge
		 *         automatically called by the public version of getHBOverCurrent
		 *
		 * \param hb halfbridge thats overcurrent flag will be returned
		 *
		 * \return 1 if there HalfBridge was shut down because of an overcurrent
		 *         0 otherwise
		 */
		uint8_t getHBOverCurrent(uint8_t hb);

		/*! \brief gets the openload error flag bit for a specific halfbridge
		 *         automatically called by the public version of getHBOpenLoad
		 *
		 * \param hb halfbridge thats openload flag will be returned
		 *
		 * \return 1 if there HalfBridge detected an open load
		 *         0 otherwise
		 */
		uint8_t getHBOpenLoad(uint8_t hb);

		/*! \brief writes data bits to a control register of the TLE9563
		 *
		 * \param reg   control register number(mapping array index / CtrlRegisters constant) of the register
		 * \param mask  mask for the bits that have to be written
		 * \param shift data will be shifted left by this amount before masking. This is for the bit alignment of data
		 * \param data  the data byte that has to be written. It will be shifted and masked before
		 *
		 * \see CtrlRegisters
		 * \see TLE9563_NUM_CTRL_REGS
		 * \see mCtrlRegAddresses
		 * \see mCtrlRegData
		 */
		void writeReg(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t data);

		/*! \brief reads one byte from a status register of the TLE9563
		 *
		 * \param reg status register number(mapping array index / StatusRegisters constant) of the register
		 *
		 * \return data byte that has been read
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		uint8_t readStatusReg(uint8_t reg);

		/*! \brief reads some bits from a status register of the TLE9563
		 *
		 * \param reg   status register number(mapping array index / StatusRegisters constant) of the register
		 * \param mask  mask for the bits that have to be read
		 * \param shift data will be shifted right by this amount after masking. This is for the bit alignment of data
		 *
		 * \return data bits that have been read (after masking and shifting)
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		uint8_t readStatusReg(uint8_t reg, uint8_t mask, uint8_t shift);

		/*! \brief clears a status register by writing 0x00 to it
		 *
		 * \param reg status register number(mapping array index / StatusRegisters constant) of the register
		 *
		 * \see StatusRegisters
		 * \see TLE9563_NUM_STATUS_REGS
		 * \see mStatusRegAddresses
		 */
		void clearStatusReg(uint8_t reg);

		uint8_t mEnabled;    //<! \brief indicates if TLE9563LE is enabled

};
/** @} */

#endif /** TLE9563_HPP_ **/
