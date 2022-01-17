.. _AGC:

Adaptive Gate Control (AGC)
===========================
The Infineon’s Motor System ICs (TLE956x) and Multi MOSFET driver ICs (TLE9210x) include a MOSFET driver with multi-stage current source gate control which is configured over SPI. Over this interface, the turn-on (tDONx) and turn-off (tDOFFx) delay can be controlled in PWM operation, as well as the rise (tRISEx) and fall (tFALLx) times. Therefore the algorithms explained in `Rise fall time regulation with current source MOSFET gate drivers`_ were implemented in this library.

.. image:: /img/AGC_animated.gif

Example codes
^^^^^^^^^^^^^^

There are two software examples available, that directly execute the regulation like shown in the animated GIF.

	* `/examples/TLE9562_DCM_rise-and-fall-time-regulation.ino <https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9562_DCM_rise-and-fall-time-regulation>`_ for use with DC motors and static loads
	* `/examples/TLE9563_BLDCM_rise-and-fall-time-regulation.ino <https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9563_BLDCM_rise-and-fall-time-regulation>`_ for use with BLDC motors

For optimal results it's recommended to use **TLE9562_DCM_rise-and-fall-time-regulation.ino** with an ideal R-L- Load instead of a real motor to avoid side effects. 
This code can be used on both TLE956x boards despite the specific name. ::

	#define HALFBRIDGE              PHASE1      // [PHASE1;Phase4] Select the phase on which you want to regulate Rise/Fall time
	#define SPEED_INCREASE_STEP     100         // [1;511] speed step increase/decrease when pressing a key
	#define CONTROL_LOOP_DELAY      400         // [ms] time between regulation executions

First choose the phase on which you want to apply the AGC algorithm.

.. doxygenenum:: _Halfbridges

All other phases will be always connected to ground. In practice it's usually sufficient to execute the algorithm on one phase, as the other MOSFETS should meet the same specifications and come from the same charge. ::

	uint8_t trise_tg = 11;                      // [0;63] Initial Risetime target. Can be changed via keyboard input.
	uint8_t tfall_tg = 11;                      // [0;63] Initial Falltime target. Can be changed via keyboard input.

Here you can define your initial target Rise- and Falltime, which will be set by

.. doxygenfunction:: DCMcontrol::setTrisefallTarget

If this function is not used (like in normal motor operation) the values from the defines will be taken as described below. The entered values in the examples are suited as a starting point for the DC and BLDC shield. However if other MOSFETS are used, refer to the `TLE9560/1/2 Gate Driver Setting Guide`_ in order to estimate start values for rise and fall times, turn-on and turn-off delay times and recommendations for the settings of the cross-current protection time and of the blank times.

.. doxygenfunction:: DCMcontrol::riseFallTimeRegulation

This function executes the algorithm one time and hands over the variables to read back the actual rise- / falltimes and charge-/discharge currents.

Parameter defines
^^^^^^^^^^^^^^^^^^
In order to constantly change the initial charge current (ICHG) / initial discharge current (IDCHG) go to `/src/corelib/TLE9xxx.hpp <https://github.com/Infineon/motor-system-ic-tle956x/blob/master/src/corelib/TLE9xxx.hpp>`_. There you find the defines listed below. Just replace the values there by the values you found out experimentally. 

.. doxygendefine:: CONF_TRISE_TG
.. doxygendefine:: CONF_INIT_ICHG
.. doxygendefine:: CONF_TFALL_TG
.. doxygendefine:: CONF_INIT_IDCHG

.. _`Rise fall time regulation with current source MOSFET gate drivers`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/#!?fileId=5546d46272e49d2a0172eaac3c9b72fb
.. _`TLE9560/1/2 Gate Driver Setting Guide`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/#!?fileId=5546d4627956d53f01798df9937b0af2