/**
 * @file        tle9563-conf.hpp
 * @brief       TLE9563 Library Configuration
 * @date        Oct 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_CONF_HPP_
#define TLE9563_CONF_HPP_

#include "tle9563-conf-opts.hpp"

/**
 * @addtogroup tle9563frmw
 * @{
 */

#ifndef TLE9563_FRAMEWORK
#define TLE9563_FRAMEWORK    TLE9563_FRMWK_ARDUINO
#endif

/**
 * @brief Logger enable
 */

#ifndef TLE9563_LOGGER_ENABLED
#define TLE9563_LOGGER_ENABLED      1           /**< (1) Enabled, (0) Disabled */
#endif

/**
 * @brief Enable for each module logger
 */

#if     (TLE9563_LOGGER_ENABLED == 1)

#define TLE9563_REG_LOGGER_ENABLED  1           /**< Register module */
#define TLE9563_APP_LOGGER_ENABLED  1           /**< Application level module */

#endif 


#include "tle9563-conf-dfl.hpp"
/** @} */

#endif /** TLE9563_CONF_HPP_ **/