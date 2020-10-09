/** 
 * \name        rampSpeedTest
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This example measure the rampspeed of an attached motor with the TLE9563 shield
 * \details
 * By attaching a motor to the TLE9563 and running a testcase scenario, we can measure the
 * optimal values for the rampSpeed function. This function allows us to smoothly start, stop,
 * raise or break the motor.
 * 
 * \attention A free running motor ramps very different than the same motor with load attached.
 * So you have to measure different scenarios.
 *
 */

#ifndef RAMPSPEEDTEST_HPP_
#define RAMPSPEEDTEST_HPP_

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

// a structure with the measured values
typedef struct {
  int16_t startspeed;
  int16_t endspeed;
  uint16_t slope;
} RampMeasurement_t, *RampMeasurement_p;

// lets define an array with different tests
#define NUM_TESTS 6
RampMeasurement_t testcases[NUM_TESTS] = {
  { 0,    255,  5000 },
  { 255,  200,  10000 },
  { 200,  0,    500 },
  { 0,    -255, 10 },
  { -255, 255,  3000 },
  { 255,  0,    1000 }
};


#endif /** TLE9563_FRAMEWORK **/
#endif /* RAMPSPEEDTEST_HPP_ */
