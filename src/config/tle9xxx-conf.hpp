/**
 * @file        tle9xxx-conf.hpp
 * @brief       TLE9XXX Library Configuration
 * @copyright   2022 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9XXX_CONF_HPP_
#define TLE9XXX_CONF_HPP_

#include "tle9xxx-conf-opts.hpp"

/**
 * @addtogroup tle9xxxfrmw
 * @{
 */

#ifndef MOTOR_SYSTEM_IC_FRAMEWORK
#define MOTOR_SYSTEM_IC_FRAMEWORK    TLE9XXX_FRMWK_ARDUINO
#endif

/**
 * @brief Logger enable
 */

#ifndef TLE9XXX_LOGGER_ENABLED
#define TLE9XXX_LOGGER_ENABLED      1           /**< (1) Enabled, (0) Disabled */
#endif

/**
 * @brief Enable for each module logger
 */

#if     (TLE9XXX_LOGGER_ENABLED == 1)

#define TLE9XXX_REG_LOGGER_ENABLED  1           /**< Register module */
#define TLE9XXX_APP_LOGGER_ENABLED  1           /**< Application level module */

#endif 


#include "tle9xxx-conf-dfl.hpp"
/** @} */

#endif /** TLE9XXX_CONF_HPP_ **/