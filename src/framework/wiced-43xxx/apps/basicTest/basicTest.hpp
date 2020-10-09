/**
 * \name        basicTest
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This example runs a basic controller test with one attached motor on the TLE9563 shield
 * \details
 * It will run the motor in forward and backward direction if the motor
 * is able to run in both directions. Otherwise it will only run in on direction.
 * Take this example as a first test when starting with this shield.
 *
 */

#ifndef BASICTEST_HPP_
#define BASICTEST_HPP_

#include "../../../../config/tle9563-conf.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

#include <stdlib.h>
#include <wiced.h>
#include <TLE9563-platf-wiced.hpp>
#include <TLE9563-pal-wiced.hpp>
#include <TLE9563.hpp>
#include <TLE9563Motor.hpp>
#include "wiced_rtos.h"

extern "C" void application_start();

#endif /** TLE9563_FRAMEWORK **/
#endif /* BASICTEST_HPP_ */
