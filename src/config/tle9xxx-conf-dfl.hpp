/**
 * @file        tle9xxx-conf-dfl.hpp
 * @brief       TLE9XXX Default Library Configuration
 * @copyright   2022 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9XXX_CONF_DFL_HPP_
#define TLE9XXX_CONF_DFL_HPP_

/**
 * @addtogroup tle9xxxfrmw
 * @{
 */

#ifndef MOTOR_SYSTEM_IC_FRAMEWORK
#error  'Motor system IC error framework undefined'    /**< Motor system IC framework must be defined in conf. or at compilation */
#endif

#ifndef TLE9XXX_LOGGER_ENABLED
#define TLE9XXX_LOGGER_ENABLED      0               

#endif

/** @} */

#endif /** TLE9XXX_CONF_DFL_HPP_ **/
