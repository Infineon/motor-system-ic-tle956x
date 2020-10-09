/*!
 * \file        TLE9563Motor.cpp
 * \name        TLE9563Motor.cpp - optionally include motor API
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This file can optionally be included in projects that use a Infineon
 *              DC Motor Control Shield with TLE9563
 *              It provides a higher abstraction for controlling motors with the TLE9563
 *              acting as an output driver
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "TLE9563Motor.hpp"

Tle9563Motor::Tle9563Motor(Tle9563 &driver)
{
	mDriver = &driver;
	mMode = COAST;
	mEnabled = false;
	mSpeed = 0u;
}

Tle9563Motor::~Tle9563Motor()
{
	end();
}

void Tle9563Motor::begin(void)
{
	mEnabled = true;
	coast();
}

void Tle9563Motor::end(void)
{
	coast();
	mEnabled = false;
}

void Tle9563Motor::initConnector(Tle9563Motor::ePolarity pol,
		Tle9563::PWMChannel channel,
		Tle9563::HalfBridge out1,
		Tle9563::HalfBridge out2,
		Tle9563::HalfBridge out3,
		Tle9563::HalfBridge out4)
{
	initConnector(pol,channel, Tle9563::TLE_FREQ80HZ, out1, out2, out3, out4);
}

void Tle9563Motor::initConnector(Tle9563Motor::ePolarity pol,
		Tle9563::PWMChannel channel,
		Tle9563::PWMFreq freq,
		Tle9563::HalfBridge out1,
		Tle9563::HalfBridge out2,
		Tle9563::HalfBridge out3,
		Tle9563::HalfBridge out4)
{
	Tle9563::HalfBridge outputs[TLE9563MOTOR_MAX_CONNECTORS] = {out1, out2, out3, out4};
	if(mEnabled == false)
	{
		mConnectors[pol].channel = channel;
		mConnectors[pol].freq = freq;
		mConnectors[pol].active_fw = false;
		for(uint8_t i = 0; i < TLE9563MOTOR_MAX_CONNECTORS; i++)
		{
			mConnectors[pol].halfbridges[i] = outputs[i];
		}
		mConnectors[pol].channel = channel;
	}
}

void Tle9563Motor::connect(Tle9563Motor::ePolarity pol, Tle9563::HalfBridge connector)
{
	if(mEnabled == false)
	{
		for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
		{
			if(mConnectors[pol].halfbridges[idx] == Tle9563::TLE_NOHB)
			{
				mConnectors[pol].halfbridges[idx] = connector;
				break;
			}
		}
	}
}

void Tle9563Motor::disconnect(Tle9563::HalfBridge connector)
{
	if(mEnabled == false)
	{
		for(uint8_t pol = 0u; pol<2u; pol++)
		{
			for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
			{
				if(connector == mConnectors[pol].halfbridges[idx])
				{
					mConnectors[pol].halfbridges[idx] = Tle9563::TLE_NOHB;
				}
			}
		}
	}
}

void Tle9563Motor::setPwm(Tle9563Motor::ePolarity pol, Tle9563::PWMChannel channel)
{
	if(mEnabled == false)
	{
		mConnectors[pol].channel = channel;
	}
}

void Tle9563Motor::setPwm(Tle9563Motor::ePolarity pol, Tle9563::PWMChannel channel, Tle9563::PWMFreq freq)
{
	if(mEnabled == false)
	{
		mConnectors[pol].channel = channel;
		mConnectors[pol].freq = freq;
	}
}

void Tle9563Motor::setPwmFreq(Tle9563Motor::ePolarity pol, Tle9563::PWMFreq freq)
{
	if(mEnabled == false)
	{
		mConnectors[pol].freq = freq;
	}
}

void Tle9563Motor::setActiveFreeWheeling(Tle9563Motor::ePolarity pol, uint8_t active_fw)
{
	if(mEnabled == false)
	{
		mConnectors[pol].active_fw = active_fw;
	}
}

void Tle9563Motor::coast()
{
	if(mEnabled == true)
	{
		mMode = COAST;
		mSpeed = 0u;
		for(uint8_t pol = 0u; pol<2u; pol++)
		{
			for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
			{
				mDriver->configHB(mConnectors[pol].halfbridges[idx],
						Tle9563::TLE_FLOATING,
						mConnectors[pol].channel,
						mConnectors[pol].active_fw);
			}
		}
	}
	mDriver->clearErrors();
}

void Tle9563Motor::stop(uint8_t force)
{
	uint8_t numHighside = 0u;
	if(mEnabled == true)
	{
		//set all outputs floating to avoid short-circuits
		coast();
		mMode = STOP;
		mSpeed = force;
		// set dutycycle depending on parameter force
		// higher force lets the motor stop quicker
		mDriver->configPWM(mConnectors[HIGHSIDE].channel, mConnectors[HIGHSIDE].freq, force);
		mDriver->configPWM(mConnectors[LOWSIDE].channel,  mConnectors[LOWSIDE].freq,  force);
		//connect highside pins to low
		for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
		{
			Tle9563::HalfBridge hb = mConnectors[HIGHSIDE].halfbridges[idx];
			if(hb != Tle9563::TLE_NOHB) {
				numHighside++;
				mDriver->configHB(hb,
						Tle9563::TLE_LOW,
						mConnectors[HIGHSIDE].channel,
						mConnectors[HIGHSIDE].active_fw);
			}
		}
		if(numHighside == 0u)
		{
			// highside is constantly connected to high.
			// also connect lowside to high to stop motor
			for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
			{
				mDriver->configHB(mConnectors[LOWSIDE].halfbridges[idx],
						Tle9563::TLE_HIGH,
						mConnectors[LOWSIDE].channel,
						mConnectors[LOWSIDE].active_fw);
			}
		}
		else
		{
			// connect all motor pins to low
			for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
			{
				mDriver->configHB(mConnectors[LOWSIDE].halfbridges[idx],
						Tle9563::TLE_LOW,
						mConnectors[LOWSIDE].channel,
						mConnectors[LOWSIDE].active_fw);
			}
		}
	}
}

void Tle9563Motor::start(int16_t speed)
{
	setSpeed(speed);
}

void Tle9563Motor::setSpeed(int16_t speed)
{
	if(mEnabled == true)
	{
		if(speed == 0)
		{
			coast();
		}
		else if(speed > 0)
		{
			mSpeed = static_cast<uint8_t>(speed);
			mDriver->configPWM(mConnectors[HIGHSIDE].channel, mConnectors[HIGHSIDE].freq, mSpeed);
			mDriver->configPWM(mConnectors[LOWSIDE].channel,  mConnectors[LOWSIDE].freq,  mSpeed);
			if(mMode != FORWARD)
			{
				//change configuration to running forward
				//set all outputs to HIGH-IMPEDANCE to avoid short-circuits
				coast();
				mMode = FORWARD;
				mSpeed = static_cast<uint8_t>(speed);
				for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
				{
					mDriver->configHB(
								mConnectors[HIGHSIDE].halfbridges[idx],
								Tle9563::TLE_HIGH,
								mConnectors[HIGHSIDE].channel,
								mConnectors[HIGHSIDE].active_fw);
					mDriver->configHB(
								mConnectors[LOWSIDE].halfbridges[idx],
								Tle9563::TLE_LOW,
								mConnectors[LOWSIDE].channel,
								mConnectors[LOWSIDE].active_fw);
				}
			}
		}
		else	// speed < 0
		{
			mSpeed = static_cast<uint8_t>(-speed);
			mDriver->configPWM(mConnectors[HIGHSIDE].channel, mConnectors[HIGHSIDE].freq, mSpeed);
			mDriver->configPWM(mConnectors[LOWSIDE].channel,  mConnectors[LOWSIDE].freq,  mSpeed);
			if(mMode != BACKWARD)
			{
				//change configuration to running backward
				//set all outputs to HIGH-IMPEDANCE to avoid short-circuits
				coast();
				mMode = BACKWARD;
				mSpeed = static_cast<uint8_t>(-speed);
				for(uint8_t idx = 0u; idx < TLE9563MOTOR_MAX_CONNECTORS; idx++)
				{
					mDriver->configHB(mConnectors[HIGHSIDE].halfbridges[idx],
							Tle9563::TLE_LOW,
							mConnectors[HIGHSIDE].channel,
							mConnectors[HIGHSIDE].active_fw);
					mDriver->configHB(mConnectors[LOWSIDE].halfbridges[idx],
							Tle9563::TLE_HIGH,
							mConnectors[LOWSIDE].channel,
							mConnectors[LOWSIDE].active_fw);
				}
			}
		}
	}
}

int16_t Tle9563Motor::getSpeed(void)
{
	int16_t ret=0;
	if(mMode == FORWARD)
	{
		ret += mSpeed;
	}
	if(mMode == BACKWARD)
	{
		ret -= mSpeed;
	}
	return ret;
}

void Tle9563Motor::rampSpeed(int16_t speed, uint16_t slope)
{
	int16_t start_speed = getSpeed();
	if (mEnabled == true && speed != start_speed)
	{
		uint32_t duration = _measureSetSpeedDuration(speed, start_speed);
		//mDriver->clearErrors();
		// calc full ramp deltas
		int16_t ramp_delta_speed = speed - start_speed;
		int16_t ramp_delta_time = slope * abs(ramp_delta_speed) / TLE9563_MAX_SPEED;
		// calc step deltas
		int16_t num_steps = ramp_delta_time / (duration - 1);
		uint16_t steptime = 0;
		// correction of step deltas for very flat ramps
		if (abs(ramp_delta_speed) < num_steps)
		{
			num_steps = abs(ramp_delta_speed);
			steptime = ramp_delta_time / abs(ramp_delta_speed) - duration;
		}
		_performSpeedStepping(start_speed, ramp_delta_speed, num_steps,steptime);
		//mDriver->clearErrors();
	}
}

uint32_t Tle9563Motor::_measureSetSpeedDuration(int16_t speed, int16_t start_speed)
{
	if(start_speed == 0)
	{
		// changing direction is additional effort
		// don't let this have an effect on the setSpeed duration measurement
		start_speed = SIGNUM(speed);
		setSpeed(start_speed);
	}
	uint32_t elapsed = 0;
	mDriver->timer->start();
	setSpeed(start_speed);
	mDriver->timer->elapsed(elapsed);
	return elapsed;
}

void Tle9563Motor::_performSpeedStepping(int16_t start_speed, int16_t ramp_delta_speed, int16_t num_steps, uint16_t steptime)
{
	uint32_t elapsed = 0; //!> none blocking delay
	mDriver->timer->start();

	if(num_steps > 0)
	{
		// normal ramp loop
		for(uint16_t i=1u; i<=num_steps; i++)
		{
			setSpeed(start_speed + ( (i*ramp_delta_speed) / num_steps ) );
			if (steptime > 0u)
			{

				while( elapsed < steptime )
				{
					mDriver->timer->elapsed(elapsed);
				}

			}
		}
	}
	else
	{
		// special case for very steep ramps
		setSpeed(start_speed+ramp_delta_speed);
	}
}
