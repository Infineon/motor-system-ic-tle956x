API
===

.. _Keyboard commands:

Keyboard commands
------------------

.. list-table::
	:header-rows: 1

	* - 
	  - Up / Enable
	  - Down / Disable
	* - Speed
	  - ``+``
	  - ``-``
	* - Motor enable
	  - ``q``
	  - ``a``
	* - Weakening  Range enable (BLDCM)
	  - ``w``
	  - ``s``
	* - Direction
	  - ``e``
	  - ``d``
	* - Risetime target
	  - ``r``
	  - ``f``
	* - Falltime target
	  - ``t``
	  - ``g``
	* - Rise- / Falltime Regulation enable
	  - ``u``
	  - ``j``
	* - AGC enable
	  - ``i``
	  - ``k``

Adaptive Gate Control (AGC)
---------------------------
The Infineon’s Motor System ICs (TLE956x) and Multi MOSFET driver ICs (TLE9210x) include a MOSFET driver with multi-stage current source gate control which is configured over SPI. Over this interface, the turn-on (tDONx) and turn-off (tDOFFx) delay can be controlled in PWM operation, as well as the rise (tRISEx) and fall (tFALLx) times. Therefore the algorithms explained in `Rise fall time regulation with current source MOSFET gate drivers`_ were implemented in this library.

.. image:: /img/AGC_animated.gif

Example codes
^^^^^^^^^^^^^^

There are two software examples available, that directly execute the regulation like shown in the animated GIF.

	* **TLE9562_DCM_rise-and-fall-time-regulation.ino** for use with DC motors
	* **TLE9563_BLDCM_rise-and-fall-time-regulation.ino** for use with BLDC motors

For optimal results it's recommended to use **TLE9562_DCM_rise-and-fall-time-regulation.ino** with an ideal R-L- Load instead of a real motor to avoid side effects. 
This code can be used on both TLE956x boards despite the specific name. 

Parameter defines
^^^^^^^^^^^^^^^^^^
In order to constantly change the initial charge current (ICHG) / initial discharge current (IDCHG) go to **TLE9xxx.hpp**.



.. _`Rise fall time regulation with current source MOSFET gate drivers`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/#!?fileId=5546d46272e49d2a0172eaac3c9b72fb