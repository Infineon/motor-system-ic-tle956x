/*!
 * \file        TLE9563Motor.hpp
 * \name        TLE9563Motor.hpp - optionally include motor API
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     2.0.0
 * \ref         tle9563corelib
 * \brief       This file can optionally be included in projects that use Infineon
 *              DC Motor Control Shield with TLE9563
 *              It provides a higher abstraction for controlling motors with the TLE9563
 *              acting as an output driver
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563MOTOR_HPP_
#define TLE9563MOTOR_HPP_

#include "TLE9563.hpp"

/**
 * @addtogroup tle9563motorapi
 * @{
 */

/**
 * @brief The maximum absolute speed value which can be set
 * 
 * @see Tle9563
 */
#define TLE9563_MAX_SPEED 255

/**
 * @brief maximum number of Tle9563 outputs per motor connector
 *
 * When this is changed, also the function signature and implementation of
 * Tle9563Motor::initConnector has to be changed.
 *
 * @see Tle9563Motor::initConnector
 */
#define TLE9563MOTOR_MAX_CONNECTORS 4

/**
 * @brief represents a motor connected to a TLE9563
 * 
 * This class provides a simple API for connecting and controlling motors.
 * Each motor is assigned to a Tle9563 which acts as output driver. Calls to
 * Tle9563Motor instances are mapped to calls to Tle9563. Therefore, this
 * class does not bring new features, it does only provide further abstraction.
 *
 * @see Tle9563
 */
class Tle9563Motor
{
	public:

		/**
		 * @brief Enum to select polarity of a motor connector
		 */
		enum ePolarity
		{
			LOWSIDE = 0,
			HIGHSIDE
		};

		/**
		 * @brief Construct a new Tle9563Motor object
		 *
		 * After constructing, the motor must be configured. You can use functions
		 * initConnector(), connect(), disconnect(), setPwm(), setPwmFreq() and
		 * setActiveFreeWheeling() for configuration.
		 * Call begin() to finish configuration and enable motor controls.
		 *
		 * @param driver Tle9563 instance acting as a driver
		 */
		Tle9563Motor(Tle9563 &driver);

		/**
		 * @brief Destroy the Tle9563Motor object
		 */
		~Tle9563Motor();

		/**
		 * @brief finishes configuration and enables motor controls
		 *
		 * For controlling the motor, you can use the functions start(), stop(),
		 * coast(), setSpeed(), getSpeed() and rampSpeed()
		 * Call end() to get back to configuration mode. After calling end() and
		 * changing configuration, you can get back to controlling mode by calling
		 * begin()
		 *
		 * @see end
		 */
		void begin(void);

		/**
		 * @brief disables motor controls and switches back to configuration mode
		 *
		 * If necessary, you can call end() and change the configuration settings
		 * at any point of time. This might be useful when sharing PWM channels
		 * between several motors. Configuration changes to motors are not allowed
		 * while controls are active!
		 * Call begin() again when the configuration changes are done to enable
		 * motor controls again.
		 *
		 * @see begin
		 */
		void end(void);

		/**
		 * @brief configures most important settings for one motor connector
		 *
		 * Call this function twice to setup motor configuration for both the
		 * + and - connector of the motor. If there is no setup for one polarity,
		 * the library assumes that the motor is constantly connected to + or -.
		 *
		 * @param pol      Polarity of the motor connector to be configured
		 * @param channel  PWM channel assigned to this connector
		 * @param out1     halfbridge output 1 of 4 assigned to this connector
		 * @param out2     halfbridge output 2 of 4 assigned to this connector
		 * @param out3     halfbridge output 3 of 4 assigned to this connector
		 * @param out4     halfbridge output 4 of 4 assigned to this connector
		 */
		void initConnector(Tle9563Motor::ePolarity pol,
			Tle9563::PWMChannel channel,
			Tle9563::HalfBridge out1,
			Tle9563::HalfBridge out2,
			Tle9563::HalfBridge out3,
			Tle9563::HalfBridge out4);

		/**
		 * @brief configures most important settings for one motor connector
		 *
		 * Call this function twice to setup motor configuration for both the
		 * + and - connector of the motor. If there is no setup for one polarity,
		 * the library assumes that the motor is constantly connected to + or -.
		 *
		 * @param pol      Polarity of the motor connector to be configured
		 * @param channel  PWM channel assigned to this connector
		 * @param freq     optional PWM frequency setting
		 * @param out1     halfbridge output 1 of 4 assigned to this connector
		 * @param out2     halfbridge output 2 of 4 assigned to this connector
		 * @param out3     halfbridge output 3 of 4 assigned to this connector
		 * @param out4     halfbridge output 4 of 4 assigned to this connector
		 */
		void initConnector(Tle9563Motor::ePolarity,
			Tle9563::PWMChannel channel,
			Tle9563::PWMFreq freq,
			Tle9563::HalfBridge out1,
			Tle9563::HalfBridge out2,
			Tle9563::HalfBridge out3,
			Tle9563::HalfBridge out4);

		/**
		 * @brief connects a single halfbridge output to a motor connector
		 * 
		 * @param pol        Polarity of the motor connector to be configured
		 * @param connector  halfbridge output to be assigned to this motor
		 */
		void connect(Tle9563Motor::ePolarity pol, Tle9563::HalfBridge connector);

		/**
		 * @brief disconnects a single halfbridge output from a motor connector
		 *
		 * @param connector  halfbridge output to be removed from this motor
		 */
		void disconnect(Tle9563::HalfBridge connector);

		/**
		 * @brief Sets the PWM channel to be used for a motor connector
		 *
		 * @param pol      Polarity of the motor connector to be configured
		 * @param channel  PWM channel to be used
		 */
		void setPwm(Tle9563Motor::ePolarity pol, Tle9563::PWMChannel channel);

		/**
		 * @brief Sets the PWM channel and frequency for a motor connector
		 *
		 * @param pol      Polarity of the motor connector to be configured
		 * @param channel  PWM channel to be used
		 * @param freq     PWM frequency selection
		 */
		void setPwm(Tle9563Motor::ePolarity pol, Tle9563::PWMChannel channel,
				Tle9563::PWMFreq freq);

		/**
		 * @brief Sets the PWM frequency for driving a motor connector
		 *
		 * @param pol     Polarity of the motor connector to be configured
		 * @param freq    PWM frequency selection
		 */
		void setPwmFreq(Tle9563Motor::ePolarity pol, Tle9563::PWMFreq freq);

		/**
		 * @brief Enables or disables active freewheeling
		 *
		 * Default is false i.e. passive freewheeling.
		 * Depending on your application circuit, active freewheeling may lead to
		 * electrical problems. Do not change this setting unless you know what
		 * you are doing.
		 * 
		 * @param pol        Polarity of the motor connector to be configured
		 * @param active_fw  true/false to enable/disable active freewheeling
		 */
		void setActiveFreeWheeling(Tle9563Motor::ePolarity pol, uint8_t active_fw);

		/**
		 * @brief actively breaks the motor to stop it
		 *
		 * @param force      higher force lets the motor stop quicker, maximum is 255
		 */
		void stop(uint8_t force=255);

		/**
		 * @brief releases the motor without driving or active breaking
		 */
		void coast();

		/**
		 * @brief starts the motor with a certain speed in a certain direction
		 *
		 * The behaviour of this function depends on the way the motor is connected
		 * to the Tle9563. Motors which have just one connector assigned to the
		 * Tle9563 cannot run in reverse direction and break instead if a negative
		 * speed value is passed.
		 * Motors without PWM assigned cannot run at a certain speed. They can just
		 * coast, run and break. Speed values greater or smaller than 0 are treated
		 * like -255 or 255, respectively.
		 *
		 * @param speed      an integer in a range from -255 to 255 to set motor speed and direction
		 */
		void start(int16_t speed);

		/**
		 * @brief equivalent to start()
		 *
		 * This function is fully equivalent to start(), but it is intended to be
		 * used if the motor is already running. (Distinguishing between these two
		 * functions helps improving the readability of your code)
		 * The behaviour of this function depends on the way the motor is connected
		 * to the Tle9563. Motors which have just one connector assigned to the
		 * Tle9563 cannot run in reverse direction and break instead if a negative
		 * speed value is passed.
		 * Motors without PWM assigned cannot run at a certain speed. They can just
		 * coast, run and break. Speed values greater or smaller than 0 are treated
		 * like -255 or 255, respectively.
		 *
		 * @param speed      an integer in a range from -255 to 255 to set motor speed and direction
		 * @see start
		 */
		void setSpeed(int16_t speed);

		/**
		 * @brief Returns the motor's current speed setting
		 *
		 * @return int16_t the motor's current speed setting
		 */
		int16_t getSpeed(void);

		/**
		 * @brief function to gradually change the motors speed
		 *
		 * @param speed    target speed
		 * @param slope    time for a full ramp from speed=0 to speed=255
		 */
		void rampSpeed(int16_t speed, uint16_t slope);

	protected:

		//! @brief enum for motor operation modes
		enum eMode
		{
			COAST,
			FORWARD,
			BACKWARD,
			STOP
		};

		//! @brief struct representing one motor connector
		typedef struct
		{
			Tle9563::HalfBridge halfbridges[TLE9563MOTOR_MAX_CONNECTORS];
			Tle9563::PWMChannel channel;
			Tle9563::PWMFreq freq;
			uint8_t active_fw;
		} Connector_t, *Connector_p;

		//! @brief array of motor connectors
		Connector_t mConnectors[2] =
		{
			{
				.halfbridges = {Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB},
				.channel = Tle9563::TLE_NOPWM,
				.freq = Tle9563::TLE_FREQ80HZ,
				.active_fw = false
			},
			{
				.halfbridges = {Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB,
						Tle9563::TLE_NOHB},
				.channel = Tle9563::TLE_NOPWM,
				.freq = Tle9563::TLE_FREQ80HZ,
				.active_fw = false
			}
		};

		//! @brief Tle9563 instance acting as output driver
		Tle9563 *mDriver;

		//! @brief current operation mode
		eMode mMode;

		//! @brief flag indicating if motor controls are enabled
		uint8_t mEnabled;

		//! @brief value of the current motor speed
		uint8_t mSpeed;

		/**
		 * @brief private function needed by rampSpeed
		 * 
		 * @param speed        integer in a range from -255 to 255 to set motor speed and direction.
		 *                     Here the target speed.
		 * @param start_speed  integer a range from -255 to 255 to set motor speed and direction.
		 *                     Here the source speed
		 */
		uint32_t _measureSetSpeedDuration(int16_t speed, int16_t start_speed);

		/**
		 * @brief private function needed by rampSpeed
		 * @param start_speed       integer a range from -255 to 255 to set motor speed
		 *                          and direction. Here the source speed
		 * @param ramp_delta_speed  slope value how fast the speed change should raise or fall
		 * @param num_steps         number of steps to raise or fall the speed
		 * @param steptime          time to be needed for each step
		 */
		void _performSpeedStepping(int16_t start_speed, int16_t ramp_delta_speed, int16_t num_steps, uint16_t steptime);
};
/** @} */

#endif /** TLE9563MOTOR_HPP_ **/
