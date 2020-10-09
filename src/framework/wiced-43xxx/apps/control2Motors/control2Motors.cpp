/*!
 * \name        control2Motors
 * \author      Infineon Technologies AG
 * \copyright   2019 Infineon Technologies AG
 * \version     2.0.0
 * \brief       This example demonstrates how to control two motors, their speed and direction with the TLE9563 shield.
 *
 * \details
 * By attaching a Tle9563 controller object to two Tle9563Motor objects, we can generate
 * an easy to handle motor object for each motor. We can easily attach this motor object
 * two any of the halfbridges and to any of the PWM units. This is done in the setup part.
 * Keep in mind that you have to initialize the Tle9563 controller and each motor object,
 * which is done in the setup function.
 * After that we can take the motor start, stop and setSpeed function to easily control
 * each motor.
 *
 * \attention Motors have depending on their size a very big latency until a motor has
 * fully started or is stopped to 0. Specially the time between a start and stop and vice versa
 * is much higher than the processing speed of any microcontroller. This is solved in this example
 * with the delay command, but there are better solutions than that.
 * These delays are needed because a direct switch between start and stop will lead to high currents
 * and the TLE9563 will signal an overcurrent.
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "../../../../config/tle9563-conf.hpp"
#include "control2Motors.hpp"

#if (TLE9563_FRAMEWORK == TLE9563_FRMWK_WICED)

// Tle9563 Object on Shield 1
Tle9563Wiced controller = Tle9563Wiced();

// Tle9563Motor Objects
Tle9563Motor motor1(controller);
Tle9563Motor motor2(controller);

void setup()
{
    // Enable MotorController Tle9563
    // Note: Required to be done before starting to configure the motor
    WPRINT_APP_INFO(("[TLE9563] : setup begin -> %u\n", 0));
    controller.begin();
    WPRINT_APP_INFO(("[TLE9563] : controller begin -> %u\n", 1));

    // Connect motor1 to HB1 and HB3
    motor1.connect(motor1.HIGHSIDE, controller.TLE_HB1);
    motor1.connect(motor1.LOWSIDE,  controller.TLE_HB3);

    // Drive HB1 with signal from PWM1
    // Note: This allows to control the speed of the motor
    motor1.setPwm(motor1.HIGHSIDE, controller.TLE_PWM1);

    // Set PWM Frequency, default is 80 Hz
    motor1.setPwmFreq(motor1.HIGHSIDE, controller.TLE_FREQ100HZ);

    // Connect motor2 to HB2 and HB4
    motor2.connect(motor2.HIGHSIDE, controller.TLE_HB2);
    motor2.connect(motor2.LOWSIDE,  controller.TLE_HB4);

    // Drive HB2 with signal from PWM2
    // Note: This allows to control the speed of the motor
    motor2.setPwm(motor2.HIGHSIDE, controller.TLE_PWM2);

    //when configuration is done, call begin to start operating the motors
    WPRINT_APP_INFO(("[TLE9563] : motor begin -> %u\n", 2));
    motor1.begin();
    motor2.begin();
    WPRINT_APP_INFO(("[TLE9563] : all ready -> %u\n", 3));
}

void loop()
{
    // start the motor1 forwards on half speed
    motor1.start(127);
    // start the motor3 backwards on half speed
    motor2.start(-127);
    wiced_rtos_delay_milliseconds(5000);

    // accelerate motor1 to full speed
    motor1.setSpeed(255);
    // stop motor2
    motor2.stop();
    wiced_rtos_delay_milliseconds(5000);

    // reduce speed of motor1
    motor1.setSpeed(127);
    wiced_rtos_delay_milliseconds(1000);

    // let motor1 turn backwards on half speed
    motor1.setSpeed(-127);
    wiced_rtos_delay_milliseconds(5000);

    // accelerate motor1 to full speed backwards
    motor1.setSpeed(-255);
    wiced_rtos_delay_milliseconds(5000);

    // stop motor1, you can use the parameter to set the force which stops and holds them.
    // standard and maximum is 255
    motor1.stop(255);
    wiced_rtos_delay_milliseconds(5000);
}



/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start( )
{
    wiced_result_t      result;

    /* Initialize the device */
    result = wiced_init();
    if (result != WICED_SUCCESS)
    {
        return;
    }

    setup();

    while(1)
    {
        loop();
    }
}

#endif /** TLE9563_FRAMEWORK **/
