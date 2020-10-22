/*!
 * \file        BLDCM_control-ino.hpp
 * \name        BLDCM_control-ino.hpp - basic motor control functions for Arduino
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This library includes the basic common functions to control a BLDC motor using an instance of TLE9563
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BLDCMCONTROLINO_HPP_
#define BLDCMCONTROLINO_HPP_

#include "config/tle9563-conf.hpp"
#include "corelib/BLDCM-control.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_ARDUINO)

/**
 * @addtogroup inoApi
 * @{
 */
class BLDCMcontrolIno: public  BLDCMcontrol
{

};

/** @} */
#endif	/** TLE9563_FRAMEWORK **/
#endif 	/** BLDCMCONTROLINO_HPP_ **/