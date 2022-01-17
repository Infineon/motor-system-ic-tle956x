.. _arduino-examples:

Arduino Examples
================
To run these examples use either the Arduino IDE or something similar like the PlatformIO extension for Visual Code or Atom.

.. list-table::
	:header-rows: 0

	* - `examples/TLE9562_DCM-control`_
	  - :ref:`Default example sketch to run DC motors. <DCM-example>`
	* - `examples/TLE9562_DCM_rise-and-fall-time-regulation`_
	  - :ref:`Perform the rise- / falltime control loop on a single output phase. <AGC>`
	* - `examples/TLE9563_BLDCM-control`_
	  - :ref:`Default example sketch to run a 3 Phase BLDC motor with Hallsensor or BEMF. <BLDCM-example>`
	* - `examples/TLE9563_BLDCM_rise-and-fall-time-regulation`_
	  - :ref:`Perform the Rise- / Falltime control loop while controlling a BLDC motor. Might be experimental. <AGC>`
	* - `examples/TLE9563_BLDCM_find-polepairs`_
	  - Simple script to identify the amount of polepairs of a connected BLDC motor. 

.. _Keyboard commands:

Keyboard commands
------------------

These are common key-operations for the serial monitor used by the various examples stated above. Not all operations are available in every example.

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


.. _`examples/TLE9562_DCM-control`: https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9562_DCM-control
.. _`examples/TLE9562_DCM_rise-and-fall-time-regulation`: https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9562_DCM_rise-and-fall-time-regulation
.. _`examples/TLE9563_BLDCM-control`: https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9563_BLDCM-control
.. _`examples/TLE9563_BLDCM_rise-and-fall-time-regulation`: https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9563_BLDCM_rise-and-fall-time-regulation
.. _`examples/TLE9563_BLDCM_find-polepairs`: https://github.com/Infineon/motor-system-ic-tle956x/tree/master/examples/TLE9563_BLDCM_find-polepairs