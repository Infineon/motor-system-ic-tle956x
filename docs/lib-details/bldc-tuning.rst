BLDC Motor Tuning
==================

If you have no or bad startup behavior with your BLDC motor, there are some parameters outside the example sketches that you can change.
Therefore navigate to `/src/corelib/BLDCM-control.hpp <https://github.com/Infineon/motor-system-ic-tle956x/blob/master/src/corelib/BLDCM-control.hpp>`_ in your local library.

Safety
--------

.. doxygendefine:: CONF_TIMEOUT

In order to prevent damage to your motor when it's mechanically blocked, there is a timeout feature. After this time (500ms) when no commutation occurred, all three phases will be switched off. 

RPM Regulation
---------------

.. doxygendefine:: CONF_PI_UPDATE_INTERVAL

Startup
-------
.. doxygendefine:: CONF_RPM_DUTYCYCLE_AT_START
.. doxygendefine:: CONF_OPEN_LOOP_DUTYCYCLE
.. doxygendefine:: CONF_OPEN_LOOP_DELAY_START
.. doxygendefine:: CONF_OPEN_LOOP_DELAY_LIMIT
.. doxygendefine:: CONF_OPEN_LOOP_DELAY_SLOPE